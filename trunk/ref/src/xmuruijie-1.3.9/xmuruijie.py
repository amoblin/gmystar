#! /usr/bin/python
# encoding: utf-8

#############################################################################
##                                                                         ##
## xmuruijie.py -- Third party Ruijie 2.56 Authentication client for Linux ##
## Version 1.3.9 2008.10.31                                                ##
##    fix static authentication bug                                        ##
##                                                                         ##
## Version 1.3.8 2008.06.10                                                ##
##    improve message exception handling                                   ##
##                                                                         ##
## Version 1.3.7 2008.04.26                                                ##
##    add zeros to tails                                                   ##
##                                                                         ##
## Version 1.3.6 2008.03.28                                                ##
##    fix message exception and use str.decode to convert charset          ##
##                                                                         ##
## Version 1.3.5 2008.03.28                                                ##
##    fix md5 challenge bug with new server                                ##
##    fix a minor locale check bug in message                              ##
##    add extra zeros as Ruijie do                                         ##
##                                                                         ##
## Version 1.3.4 2008.01.20                                                ##
##    modify the md5 response                                              ##
##                                                                         ##
## Version 1.3.3 2007.11.24                                                ##
##    modify xrgsu algorithm                                               ##
##                                                                         ##
## Version 1.3.2 2007.10.09                                                ##
##    improve server message parse algorithm -- thanks Lv Gaohan           ##
##    fix package receive mac insufficiency -- thanks Lv Gaohan            ##
##                                                                         ##
## Version 1.3.1 2007.9.04                                                 ##
##    change default configuration file to /etc/xmuruijie.conf             ##
##                                                                         ##
## Version 1.3.0 2007.8.30                                                 ##
##    add xrgsu 1.1.1 algorithm                                            ##
##    add server message interpreter for utf8                              ##
##                                                                         ##
## Version 1.2.1 2007.8.25                                                 ##
##    improve ruijie.ping() from ping DNS to ping gateway.                 ##
##                                                                         ##
## Version 1.2 2007.8.24                                                   ##
##    add logoff packet support                                            ##
##    add a fallback boardcast address for HaiYun                          ##
##                                                                         ##
## Version 1.1 2007.8.20                                                   ##
##                                                                         ##
## Copyright (C) 2007 YuYuwei <yu-yuwei@xmu.edu.cn>                        ##
##                                                                         ##
## Writen and tested under Gentoo Linux 2007.0                             ##
##                                                                         ##
## The EAP part(Not include MD5-Challenge) is based on eapy.py             ##
##   by Philippe Biondi <biondi@cartel-securite.fr>                        ##
##                                                                         ##
## The Red Gaint authentication algorithm is based on Mento Supplciant-6.0 ##
##   by Soar                                                               ##
##                                                                         ##
## This program is free software; you can redistribute it and/or modify it ##
##   under the terms of the GNU General Public License version 2           ##
##                                                                         ##
## This program is distributed in the hope that it will be useful, but     ##
##   WITHOUT ANY WARRANTY; without even the implied warranty of            ##
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     ##
##   General Public License for more details.                              ##
##                                                                         ##
#############################################################################

### Import modules we need
import os
import socket
import sys
import optparse
import ConfigParser
import md5
import locale
from struct import pack,unpack
from time import sleep
from binascii import b2a_hex
from signal import SIGINT
from random import randint
from commands import getoutput,getstatusoutput
from fcntl import ioctl



### Constants

ETHERTYPE_PAE = 0x888e
PAE_GROUP_ADDR = "\x01\xd0\xf8\x00\x00\x03" # Ruijie d boardcast addr
PAE_GROUP_ADDR_2 = "\x01\x80\xc2\x00\x00\x03" 

EAPOL_VERSION = 1
EAPOL_EAPPACKET = 0
EAPOL_START = 1
EAPOL_LOGOFF = 2
EAPOL_KEY = 3
EAPOL_ASF = 4
EAPOL_RG = 0xbf

EAP_REQUEST = 1
EAP_RESPONSE = 2
EAP_SUCCESS = 3
EAP_FAILURE = 4

EAP_TYPE_ID = 1
EAP_TYPE_MD5 = 4

### Option Parse:
opt = optparse.OptionParser()
opt.set_usage('''
	Start in Foreground: %prog 
	Start as Daemon: %prog -d
	Stop when running in daemon: %prog -p''')
opt.add_option('--config', '-c',
		action = 'store',type = 'string',dest = 'conf',default = '/etc/xmuruijie.conf',
		help = 'Set the configuration file, default: %default')
opt.add_option('--daemon','-d',
		action = 'store_true',dest = 'daemon',default=False,
		help = 'Run as daemon, default: %default')
opt.add_option('--start','-s',
		action = 'store_true',dest = 'action',default = True,
		help = 'Start Authentication')
opt.add_option('--static-mode','-S',
		action = 'store_true',dest = 'static',default = False,
		help = 'Static Authentication mode')
opt.add_option('--dhcp-mode','-D',
		action = 'store_false',dest = 'static',
		help = 'DHCP Authentication mode')
opt.add_option('--stop','-p',
		action = 'store_false', dest= 'action',
		help = 'Stop Authentication')
opt.add_option('--xrgsu','-x',
		action = 'store_true',dest = 'xrgsu', default=False,
		help = 'Use xrgsu-1.1.1 algorithm, default: %default')
opt.add_option('--md5other','-m',
		action = 'store_true',dest = 'md5o', default=False,
		help = 'Use a changed type of md5 response in some school, default: %default')


(options, args) = opt.parse_args()


### ConfigFile Parse:

conf = ConfigParser.SafeConfigParser()
try:
	conf.readfp( file(options.conf,'r'))
except IOError, err:
	s = 'Fail to open configuration file--"%s": "%s"' % ( err.filename, err.strerror)
	sys.stderr.write(s)
	sys.stderr.flush()
	sys.exit(1)
###  Authentication parameters

USER = conf.get('auth','user')
PASS = conf.get('auth','passwd')
DEV = conf.get('auth','interface')
logfile = conf.get('daemon','logfile')
pidfile = conf.get('daemon','pid')


### Packet builders

def EAPOL(type, payload=""):
	return pack("!BBH", EAPOL_VERSION, type, len(payload))+payload

def EAP(code, id, type=0, data=""):
	if code in [EAP_SUCCESS, EAP_FAILURE]:
		return pack("!BBH", code, id, 4)
	else:
		return pack("!BBHB", code, id, 5+len(data), type)+data

def ethernet_header(src, dst, type):
	return dst+src+pack("!H",type)

def printframe( data ):
	length = len(data)
	lines = length / 16.0
	lineno = 0
	for i in range(0,length):
		if ( i % 0x10 ) == 0:
			print '\n%04x:' % lineno,
			lineno += 1
		print b2a_hex(data[i]),
	print

def add_zeros( data, nums=1000 ):
	if not options.xrgsu:
		# add zeros to send frame to make it to 1000bit as RuijieSupplicant
		len_need = nums - len(data)
		zeros = ''
		if len_need:
			for i in range(len_need):
				zeros += pack('!B',0)
		return data + zeros
	else:
		return data



### Daemon section

def daemonize(stdout='/dev/null', stderr=None, stdin='/dev/null',
		pidfile=None ):
	'''
	This forks the current process into a daemon.
	The stdin, stdout, and stderr arguments are file names that
	will be opened and be used to replace the standard file descriptors
	in sys.stdin, sys.stdout, and sys.stderr.
	These arguments are optional and default to /dev/null.
	Note that stderr is opened unbuffered, so
	if it shares a file with stdout then interleaved output
	may not appear in the order that you expect.
	'''
	# Do first fork.
	try: 
		pid = os.fork() 
		if pid > 0: sys.exit(0) # Exit first parent.
	except OSError, e: 
		sys.stderr.write("fork #1 failed: (%d) %s\n" % (e.errno, e.strerror))
		sys.exit(1)

	# Decouple from parent environment.
	os.chdir("/") 
	os.umask(0) 
	os.setsid() 

	# Do second fork.
	try: 
		pid = os.fork()
		if pid > 0: sys.exit(0) # Exit second parent.
	except OSError, e: 
		print 'second fork error'
		sys.stderr.write("fork #2 failed: (%d) %s\n" % (e.errno, e.strerror))
		sys.exit(1)

	# Open file descriptors and print start message
	if not stderr: stderr = stdout
	si = file(stdin, 'r')
	so = file(stdout, 'w+')
	se = file(stderr, 'a+', 0)
	pid = str(os.getpid())
	print "Start with Pid: %s\n"  % pid
	sys.stderr.flush()
	if pidfile: file(pidfile,'w+').write("%s\n" % pid)

	# Redirect standard file descriptors.
	sys.stdout.flush()
	sys.stderr.flush()
	os.dup2(si.fileno(), sys.stdin.fileno())
	os.dup2(so.fileno(), sys.stdout.fileno())
	os.dup2(se.fileno(), sys.stderr.fileno())

def startstop( action, pidfile='/var/run/xmuruijie.pid',
		stdout='/dev/null', stderr=None, stdin='/dev/null' ):
	try:
		pf  = file(pidfile,'r')
		pid = int(pf.read().strip())
		pf.close()
	except IOError:
		pid = None
	if 'stop' == action or 'restart' == action:
		if not pid:
			mess = "Could not stop, pid file '%s' missing.\n"
			sys.stderr.write(mess % pidfile)
			if 'stop' == action:
				sys.exit(1)
			action = 'start'
			pid = None
		else:
			try:
				while 1:
					os.kill(pid,SIGINT)
					sleep(1)
			except OSError, err:
				err = str(err)
				if err.find("No such process") > 0:
					os.remove(pidfile)
					if 'stop' == action:
						sys.exit(0)
					action = 'start'
					pid = None
				else:
					print str(err)
					sys.exit(1)
	if 'start' == action:
		if pid:
			mess = "Start aborded since pid file '%s' exists.\n"
			sys.stderr.write(mess % pidfile)
			sys.exit(1)
		daemonize( stdout,stderr,stdin,pidfile )
		return

### Red Gaint Algorithm

class ruijie:
	
	RJ_DHCP_OFFSET_1 = 4
	RJ_IP_OFFSET = 5
	RJ_NM_OFFSET = 9
	RJ_GW_OFFSET = 13
	RJ_DNS_OFFSET = 17
	RJ_CS_OFFSET = 21
	RJ_HASH_OFFSET = 78
	RJ_DHCP_OFFSET_2 = 121
	RJ_MAC_OFFSET = 130
	
	def __init__(self,interface,md5str=''):
		self.iface = interface
		self.s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
		self.buf = pack('256s',interface)
		self.IP = 0x8915 # SIOCGIFADDR
		self.NW = 0x891b # SIOCGIFNETMASK
		self.HW = 0x8927 # SIOCGIFHWADDR
		self._mac = self.mac()
		self.md5rec = md5str
		self.sserial = 0x102a

	def get(self):
		'''
		Return a tuple ( boolen, (integer1,integer2,...integer16) ).
		The boolen tell you whether the given interface is up( have IP and so on),
		the integer tuple is (IP,SubNetmask,Gateway,FirstDNS)

		'''
		if self.ip() and self.netmask() and self.gateway() and self.dns():
			return ( True,unpack('!16B', self.ip() + self.netmask() + self.gateway() + self.dns() ) )
		else:
			return ( False,(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0) )

	def ip(self):
		'''
		return the local IP of given interface in binary
		'''
		try:
			return ioctl(self.s.fileno(),self.IP,self.buf)[20:24]
		except IOError:
			return ''
	
	def netmask(self):
		'''
		return the SubNetmask of given interface in binary
		'''
		try:
			return ioctl(self.s.fileno(),self.NW,self.buf)[20:24]
		except IOError:
			return ''

	def gateway(self):
		'''
		return the Default Gateway of given interface in binary
		'''
		gw = getoutput( "netstat -rn | grep  '%s' | grep 'UG' | awk '{print $2}'" % self.iface )
		if gw:
			return socket.inet_aton( gw )
		else:
			return ''
	
	def dns(self):
		'''
		return the First DNS in binary
		'''
		dns_d = getoutput("cat /etc/resolv.conf | sed -n '/^nameserver/{p;q}' | awk '{print $2}'")
		if dns_d:
			return socket.inet_aton( dns_d )
		else:
			return ''
		
	def mac(self):
		'''
		return the MAC of given interface in binary
		'''
		try:
			hwaddr = ioctl(self.s.fileno(),self.HW,self.buf)[18:24]
			return unpack( '!6B',hwaddr )
		except IOError:
			print 'The given interface %s is not exist use "00:00:00:00:00:00" as its mac! ' % self.iface
			return (0,0,0,0,0,0)


	def givemd5( self, getmd5 ):
		'''
		Update md5rec.
		'''
		self.md5rec = getmd5

	def checksumNP( self, sCircleBase ):
		'''Red Gaint's algorithm to checksum your net-parameters.
		It take a 21 elements-list as its circlebase, use bitwise operation to 
		calculate the checksums.
		take netparas as an example:
		netparas =[
			# UnEncode( 0x00,0x00,0x13,0x11 ) Ruijie OEM Mark:
			0x00,0x00,0x13,0x11,
			# EncodeMark( 0x01/00 ):
			0x01,
			# UnEncode( IP ):
			0x00,0x00,0x00,0x00, 
			# UnEncode( SubNetMask ):
			0x00,0x00,0x00,0x00, 
			# UnEncode( NetGate ):
			0x00,0x00,0x00,0x00,
			# UnEncode( DNS ):
			0x00,0x00,0x00,0x00
			]
			Then return the two checksums
			'''
		table=[	0x00,0x00,0x21,0x10,0x42,0x20,0x63,0x30,0x84,0x40,
				0xA5,0x50,0xC6,0x60,0xE7,0x70,0x08,0x81,0x29,0x91,
				0x4A,0xA1,0x6B,0xB1,0x8C,0xC1,0xAD,0xD1,0xCE,0xE1,
				0xEF,0xF1,0x31,0x12,0x10,0x02,0x73,0x32,0x52,0x22,
				0xB5,0x52,0x94,0x42,0xF7,0x72,0xD6,0x62,0x39,0x93,
				0x18,0x83,0x7B,0xB3,0x5A,0xA3,0xBD,0xD3,0x9C,0xC3,
				0xFF,0xF3,0xDE,0xE3,0x62,0x24,0x43,0x34,0x20,0x04,
				0x01,0x14,0xE6,0x64,0xC7,0x74,0xA4,0x44,0x85,0x54,
				0x6A,0xA5,0x4B,0xB5,0x28,0x85,0x09,0x95,0xEE,0xE5,
				0xCF,0xF5,0xAC,0xC5,0x8D,0xD5,0x53,0x36,0x72,0x26,
				0x11,0x16,0x30,0x06,0xD7,0x76,0xF6,0x66,0x95,0x56,
				0xB4,0x46,0x5B,0xB7,0x7A,0xA7,0x19,0x97,0x38,0x87,
				0xDF,0xF7,0xFE,0xE7,0x9D,0xD7,0xBC,0xC7,0xC4,0x48,
				0xE5,0x58,0x86,0x68,0xA7,0x78,0x40,0x08,0x61,0x18,
				0x02,0x28,0x23,0x38,0xCC,0xC9,0xED,0xD9,0x8E,0xE9,
				0xAF,0xF9,0x48,0x89,0x69,0x99,0x0A,0xA9,0x2B,0xB9,
				0xF5,0x5A,0xD4,0x4A,0xB7,0x7A,0x96,0x6A,0x71,0x1A,
				0x50,0x0A,0x33,0x3A,0x12,0x2A,0xFD,0xDB,0xDC,0xCB,
				0xBF,0xFB,0x9E,0xEB,0x79,0x9B,0x58,0x8B,0x3B,0xBB,
				0x1A,0xAB,0xA6,0x6C,0x87,0x7C,0xE4,0x4C,0xC5,0x5C,
				0x22,0x2C,0x03,0x3C,0x60,0x0C,0x41,0x1C,0xAE,0xED,
				0x8F,0xFD,0xEC,0xCD,0xCD,0xDD,0x2A,0xAD,0x0B,0xBD,
				0x68,0x8D,0x49,0x9D,0x97,0x7E,0xB6,0x6E,0xD5,0x5E,
				0xF4,0x4E,0x13,0x3E,0x32,0x2E,0x51,0x1E,0x70,0x0E,
				0x9F,0xFF,0xBE,0xEF,0xDD,0xDF,0xFC,0xCF,0x1B,0xBF,
				0x3A,0xAF,0x59,0x9F,0x78,0x8F,0x88,0x91,0xA9,0x81,
				0xCA,0xB1,0xEB,0xA1,0x0C,0xD1,0x2D,0xC1,0x4E,0xF1,
				0x6F,0xE1,0x80,0x10,0xA1,0x00,0xC2,0x30,0xE3,0x20,
				0x04,0x50,0x25,0x40,0x46,0x70,0x67,0x60,0xB9,0x83,
				0x98,0x93,0xFB,0xA3,0xDA,0xB3,0x3D,0xC3,0x1C,0xD3,
				0x7F,0xE3,0x5E,0xF3,0xB1,0x02,0x90,0x12,0xF3,0x22,
				0xD2,0x32,0x35,0x42,0x14,0x52,0x77,0x62,0x56,0x72,
				0xEA,0xB5,0xCB,0xA5,0xA8,0x95,0x89,0x85,0x6E,0xF5,
				0x4F,0xE5,0x2C,0xD5,0x0D,0xC5,0xE2,0x34,0xC3,0x24,
				0xA0,0x14,0x81,0x04,0x66,0x74,0x47,0x64,0x24,0x54,
				0x05,0x44,0xDB,0xA7,0xFA,0xB7,0x99,0x87,0xB8,0x97,
				0x5F,0xE7,0x7E,0xF7,0x1D,0xC7,0x3C,0xD7,0xD3,0x26,
				0xF2,0x36,0x91,0x06,0xB0,0x16,0x57,0x66,0x76,0x76,
				0x15,0x46,0x34,0x56,0x4C,0xD9,0x6D,0xC9,0x0E,0xF9,
				0x2F,0xE9,0xC8,0x99,0xE9,0x89,0x8A,0xB9,0xAB,0xA9,
				0x44,0x58,0x65,0x48,0x06,0x78,0x27,0x68,0xC0,0x18,
				0xE1,0x08,0x82,0x38,0xA3,0x28,0x7D,0xCB,0x5C,0xDB,
				0x3F,0xEB,0x1E,0xFB,0xF9,0x8B,0xD8,0x9B,0xBB,0xAB,
				0x9A,0xBB,0x75,0x4A,0x54,0x5A,0x37,0x6A,0x16,0x7A,
				0xF1,0x0A,0xD0,0x1A,0xB3,0x2A,0x92,0x3A,0x2E,0xFD,
				0x0F,0xED,0x6C,0xDD,0x4D,0xCD,0xAA,0xBD,0x8B,0xAD,
				0xE8,0x9D,0xC9,0x8D,0x26,0x7C,0x07,0x6C,0x64,0x5C,
				0x45,0x4C,0xA2,0x3C,0x83,0x2C,0xE0,0x1C,0xC1,0x0C,
				0x1F,0xEF,0x3E,0xFF,0x5D,0xCF,0x7C,0xDF,0x9B,0xAF,
				0xBA,0xBF,0xD9,0x8F,0xF8,0x9F,0x17,0x6E,0x36,0x7E,
				0x55,0x4E,0x74,0x5E,0x93,0x2E,0xB2,0x3E,0xD1,0x0E,
				0xF0,0x1E ]
		ax=0
		bx=0
		dx=0
	
		for i in range(0,0x15):
			dx = ax
			bx = 0
			bx = (bx&0xff00) | (sCircleBase[i])
			dx &= 0xffff
			dx >>= 8
			dx ^= bx
			bx = 0
			bx &= 0x00ff
			bx |= (ax&0xff)<<8
			ax = table[dx*2] | table[dx*2+1]<<8
			ax ^= bx
		
		rjcs0 =  ((ax&0xff00)>>8)
		rjcs1 =  (ax&0x00ff)
		return (rjcs0,rjcs1)

	def encodeNP( self, oriNP ):
		'''Red Gaint's algorithm to encode the net parameters.
		'''
		a = b = c = d = 0
		a = oriNP
		c = a
		d = a
		c &= 0x40
		b = a
		d >>= 2
		c |= d
		d = a
		c >>= 2
		d &= 0x20
		c |= d
		d = a
		c >>= 2
		d &= 0x10
		c |= d
		
		d = a
		d &= 2
		b <<= 2
		d |= b
		b = a
		d <<= 2
		b &= 4
		a &= 8
		d |= b
		d <<= 2
		d |= a
		
		c >>= 1
		d <<= 1
		c |= d
		iRt = (~c) & 0xff
		return iRt

	def randstr( self ):
		'''Special random string function for the seek frame and first Respond frame.
		Comes from Mento Supplicant v 6.0 beta3 by soar.
	
		'''
		a = randint(0,0xffff)
		b = randint(0,0xffff)
		c = randint(0,0xffff)
		d = randint(0,0xffff)
		e = randint(0,0xffff)
		f = randint(0,0xffffffff)
		interstr = '%X%X%X%X%X%X388498639F49EBACA773DFD78789088A' % (a,b,c,d,e,f )
		return unpack( '!32B', interstr[0:32] )

	def Hash8021x(self, md5get ): # md5get is a binary string 
		'''
		Hash8021x is the key algorithm in Red Gaint (Ruijie) Supplicant. 
		It comes from Mento Supplicant v 6.0 beta 3 by soar.
	
		'''
		# this 8021x.exe is RuiJie Supplicant 2.56
		rjfile = file( '/etc/8021x.exe', 'rb')
		rjfile.seek(0x1000)
		bufferSize = 0x4a00
		tableC='' # the string to be digest
		
		for i in range(0,8):
			tableC +=  md5get[i*2] 
			h_md5rec=md5get
			m2=  md5.new( h_md5rec + rjfile.read(bufferSize))
			tableC += m2.digest()
			tableC += md5get[i*2+1] 
		
		strMD5Hash = md5.new( tableC ).hexdigest()
		return unpack( '!32B', strMD5Hash )


	def tail(self):
		self.rgsig = [
				## 0 --> 22
				0x00,0x00,0x13,0x11,	# Encode( 0x00,0x00,0x13,0x11 ) Ruijie OEM Mark
				0x01,					# Encode( 0x01/00 ) DHCP Enable Flag
				0x00,0x00,0x00,0x00,	# Encode( IP )
				0x00,0x00,0x00,0x00,	# Encode( SubNetMask )
				0x00,0x00,0x00,0x00, # Encode( NetGate )
				0x00,0x00,0x00,0x00, # Encode( DNS )
				0x00,0x00, # Checksum( )
				## 23 --> 58
				0x00,0x00,0x13,0x11,0x38,0x30,0x32,0x31,0x78,0x2e,0x65,0x78,0x65,0x00,0x00,0x00, # ASCII 8021x.exe
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, #
				0x00,0x00,0x00,0x00,
				## 59 --> 77
				0x02,0x38,0x00,0x00, # 8021x.exe File Version (2.56)
				0xa0, # unknow flag
				0x00,0x00,0x13,0x11,0x00,0x4a,0x1a,0x28,0x00,0x00,0x13,0x11,0x17,0x22, # Const strings
				## 78 --> 120
				0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f, 
				0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f, # 32bytes spc. Random strings
				0x1a,0x0c,0x00,0x00,0x13,0x11,0x18,0x06,0x00,0x00,0x00, # Const strings
				## 121
				0x01, # DHCP and first time flag
				# V2.56 (and upper?) added
				## 122 -->
				0x1a,0x0e,0x00,0x00,0x13,0x11,0x2d,0x08, # Const strings
				## 130 --> 135
				0x00,0x00,0x00,0x00,0x00,0x00, # True NIC MAC
				## 136--> 143
				0x1a,0x08,0x00,0x00,0x13,0x11,0x2f,0x02 # Const strings
				]
		# now let's fill the tail :-)
		# 1 , we fill in the Network Parameters part:
		## 5 --> 20
		if options.static:
			print "Static mode"
			self.rgsig[ruijie.RJ_DHCP_OFFSET_1]=0x00
		else:
			print "DHCP mode"
			self.rgsig[ruijie.RJ_DHCP_OFFSET_1]=0x01

		updown, netPara = self.get()
		
		if updown:
			print "The %s is up, we will use Network Parameters for Authentication.\n" % self.iface
			self.rgsig[ruijie.RJ_DHCP_OFFSET_2]=0x00
			
		else:
			print "The %s is not up, we don't use Network Parameters for Authentication.\n" % self.iface
			self.rgsig[ruijie.RJ_DHCP_OFFSET_2]=0x01

		for i in range(0,16):
			self.rgsig[ruijie.RJ_IP_OFFSET+i] = netPara[i]


		# 2 , we calculate the Checksum of the Network Parameters.
		##  21 --> 22
		self.rgsig[ruijie.RJ_CS_OFFSET],self.rgsig[ruijie.RJ_CS_OFFSET+1] = self.checksumNP(self.rgsig[0:21])

		# 3 , we Encode the Network Parameter in Ruijie's algorithm.
		#  0 -- > 22
		for i in range(0,23):
			self.rgsig[i] = self.encodeNP(self.rgsig[i])
		
	
		# 4 , we generate the sepecial 32 byte Hash
		## 78 --> 109
		if not self.md5rec:
			print "We haven't got the random md5 hash, so use randstr() to fill the 32 byte Hash.\n"
			rghash = self.randstr()
		else:
			print "Use the got random md5 hash to generate the 32 byte Hash.\n"
			print 'Got md5hash is: %s' % b2a_hex(self.md5rec)
			rghash = self.Hash8021x(self.md5rec)
		for i in range(0,32):
			self.rgsig[ ruijie.RJ_HASH_OFFSET+i ] = rghash[i]

		# 5 , we fill the NIC MAC
		## 130 --> 135
		for i in range(0,6):
			self.rgsig[ ruijie.RJ_MAC_OFFSET + i ] = self._mac[i]
		
		# now we pack the rgsig into string:
		
		self.rgsig_str=''
		for byte in self.rgsig:
			self.rgsig_str += pack( '!B', byte )

		# All right, the Ruijie tail is ready :)
		
		return self.rgsig_str

	def ssid( self, recvdata ):
		'''get Red Gaint SSID from the whole successful frame, and return it in 16-bit integer.
		'''
		# eaplen:
		offset = unpack( '!H',recvdata[0x10:0x12] )[0] 
		print 'header length: %d' % offset
		
		# the first 8-bit integer index:
		rg_index = 0x12 + offset -7 
		
		print 'Got Red Gaint SSID:',
		for i in range(rg_index,rg_index+2):
			print b2a_hex(recvdata[ i] ),
		print 
		# string to store the packed 8-bit integers:
		_ssid_ = '' 
		
		# form encoded ssid :
		for i in range( rg_index, rg_index+2 ): 
			# unpack the integer first, then use encodeNP to process it,
			# and then pack it to ssid:
			_ssid_ += pack( '!B', self.encodeNP( unpack( '!B',recvdata[i] )[0] ) ) 
		
		return unpack('!H',_ssid_ )[0]


	def alive( self, rg_ssid):
		'''build the echo package to sent the online status.'''
		rgalivebit = [ 0xff, 0xff, 0x37, 0x77, 0x7f, 0x9f, 0x27, 0x2e, 0x00, 0x00,
				0xff, 0xff, 0x37, 0x77, 0x7f, 0x9f, 0x27, 0x2e, 0x00, 0x00,
				0xff, 0xff, 0x37, 0x77, 0x7f, 0x3f, 0xff, 0x00, 0x00, 0x00 ]
		
		self.sserial += 1
		uCrypt1 = rg_ssid + self.sserial
		uCrypt2 = self.sserial
		
		uCrypt1_hex = unpack( "!BB", pack( '!H', uCrypt1 ) )
		uCrypt2_hex = unpack( "!BB", pack( '!H', uCrypt2 ) )
		
		rgalivebit[0x08] = self.encodeNP(uCrypt1_hex[0])
		rgalivebit[0x09] = self.encodeNP(uCrypt1_hex[1])
		rgalivebit[0x12] = self.encodeNP(uCrypt2_hex[0])
		rgalivebit[0x13] = self.encodeNP(uCrypt2_hex[1])
	
		rg_alive=''
		for i in rgalivebit:
			rg_alive += pack('!B', i )
		
		print 'now the sserial is %04x' % self.sserial
	
		return rg_alive

	def ping( self ):
		gw = socket.inet_ntoa( self.gateway() )
		print 'It is time to ping gateway:'
		online, output = getstatusoutput( 'ping -c 3 %s' %gw )
		if online == 0:
			print '    We are online:-)'
			return True

		else:
			print '    We are not online:-('
			return False

	def message( self, res_frame ):
		try:
			_msg_ = res_frame.split('\x13\x11')[1].strip('\x00').strip("'").strip('\r').strip('\n')
		except IndexError:
			print '* No message from server *'
		else:
			if len(_msg_) == 0:
				print '* No message from server *'
			else:
				try:
					_enc_ = locale.getdefaultlocale()[1].lower()
				except:
					# use utf8 as default charset
					_enc_ = 'utf8'
				print '!* Got message from server: *!'
				print 'The enc of this machine is %s' % _enc_
				try:
					if _enc_ == 'utf8' or _enc_ == 'utf-8':
						# we decode gb18030 string from serve to unicode
						print 'decoded _msg_ to unicode.'
						_msg_ = '\t' + _msg_.decode('gb18030').encode('utf8')
					else:
						# machine do not use utf8, thus, need not
						# convert string
						_msg_ = '\t' + _msg_
					print _msg_
				except:
					print "we encount _msg_ error, could not display the message, sorry:("


class xrgsu(ruijie):
	XRGSU_DHCP_OFFSET_2 = 116
	def __init__(self,interface,md5str=''):
		self.iface = interface
		self.s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
		self.buf = pack('256s',interface)
		self.IP = 0x8915 # SIOCGIFADDR
		self.NW = 0x891b # SIOCGIFNETMASK
		self.HW = 0x8927 # SIOCGIFHWADDR
		self._mac = self.mac()
		self.md5rec = md5str
		self.sserial = 0x102a

	def tail(self):
		self.rgsig = [
				## 0 --> 22
				0x00,0x00,0x13,0x11,	# Encode( 0x00,0x00,0x13,0x11 ) Ruijie OEM Mark
				0x01,					# Encode( 0x01/00 )
				0x00,0x00,0x00,0x00,	# Encode( IP )
				0x00,0x00,0x00,0x00,	# Encode( SubNetMask )
				0x00,0x00,0x00,0x00, # Encode( NetGate )
				0x00,0x00,0x00,0x00, # Encode( DNS )
				0x00,0x00, # Checksum( )
				## 23 --> 58
				0x00,0x00,0x13,0x11,0x38,0x30,0x32,0x31,0x78,0x2e,0x65,0x78,0x65,0x00,0x00,0x00, # ASCII 8021x.exe
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, #
				0x00,0x00,0x00,0x00,
				## 59 --> 77
				0x01,0x00,0x01,0x00, # 8021x.exe File Version (1.0.1)
				0x00, # unknow flag
				0x00,0x00,0x13,0x11,0x00,0x28,0x1a,0x28,0x00,0x00,0x13,0x11,0x17,0x22, # Const string
				# 78 --> 120
				0x91,0x66,0x64,0x93,0x67,0x60,0x65,0x62,0x62,0x94,0x61,0x69,0x67,0x86,0x94,0x98, 
				0x92,0x6e,0x66,0x93,0x92,0x6f,0x9d,0x68,0x62,0x95,0x65,0x99,0x92,0x69,0x96,0x6a, # 32bytes spc. Const string
				0x00,0x00,0x13,0x11,0x18,0x06, # Const strings
				## 116 DHCP and first time flag
				0x01,0x00,0x00,0x00
				]
		
		# now let's fill the tail :-)
		# 1 , we fill in the Network Parameters part:
		## 5 --> 20
		if options.static:
			print "Static mode"
			self.rgsig[ruijie.RJ_DHCP_OFFSET_1]=0x00
		else:
			print "DHCP mode"
			self.rgsig[ruijie.RJ_DHCP_OFFSET_1]=0x01

		updown, netPara = self.get()
		
		if updown:
			print "The %s is up, we will use Network Parameters for Authentication.\n" % self.iface
			self.rgsig[xrgsu.XRGSU_DHCP_OFFSET_2]=0x00
			
		else:
			print "The %s is not up, we don't use Network Parameters for Authentication.\n" % self.iface
			self.rgsig[xrgsu.XRGSU_DHCP_OFFSET_2]=0x01
		
		for i in range(0,4):
			self.rgsig[ruijie.RJ_IP_OFFSET+i] = netPara[i]


		# 2 , we calculate the Checksum of the Network Parameters.
		##  21 --> 22
		self.rgsig[ruijie.RJ_CS_OFFSET],self.rgsig[ruijie.RJ_CS_OFFSET+1] = self.checksumNP(self.rgsig[0:21])

		# 3 , we Encode the Network Parameter in Ruijie's algorithm.
		#  0 -- > 22
		for i in range(0,23):
			self.rgsig[i] = self.encodeNP(self.rgsig[i])
		
		# now we pack the rgsig into string:
		
		self.rgsig_str=''
		for byte in self.rgsig:
			self.rgsig_str += pack( '!B', byte )

		# All right, the Ruijie tail is ready :)
		
		return self.rgsig_str

	def alive( self, rg_ssid):
		'''build the echo package to sent the online status.'''
		rgalivebit = [ 0xff, 0xff, 0x37, 0x77, 0x7f, 0x9f, 0x27, 0x2e, 0x00, 0x00,
				0xff, 0xff, 0x37, 0x77, 0x7f, 0x9f, 0x27, 0x2e, 0x00, 0x00,
				0xff, 0xff, 0x37, 0x77, 0x7f, 0x3f, 0xff, 0x00, 0x00, 0x00 ]
		
		self.sserial += 1
		uCrypt1 = rg_ssid + self.sserial
		uCrypt2 = self.sserial
		
		uCrypt1_hex = unpack( "!BB", pack( '!H', uCrypt1 ) )
		uCrypt2_hex = unpack( "!BB", pack( '!H', uCrypt2 ) )
		
		rgalivebit[0x08] = self.encodeNP(uCrypt1_hex[0])
		rgalivebit[0x09] = self.encodeNP(uCrypt1_hex[1])
		rgalivebit[0x12] = self.encodeNP(uCrypt2_hex[0])
		rgalivebit[0x13] = self.encodeNP(uCrypt2_hex[1])
	
		rg_alive=''
		for i in rgalivebit:
			rg_alive += pack('!B', i )
		
		print 'now the sserial is %04x' % self.sserial
	
		return rg_alive


### Main program/
def main():
	s=socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.htons(ETHERTYPE_PAE))
	s.bind((DEV, ETHERTYPE_PAE))
	# the state of interface before 
	if_orig_up = True
	
	mymac = s.getsockname()[4]
	inithead = ethernet_header(mymac, PAE_GROUP_ADDR, ETHERTYPE_PAE)
	inithead_2 = ethernet_header(mymac, PAE_GROUP_ADDR_2, ETHERTYPE_PAE)
	if not options.xrgsu:
		print 'we use Ruijie 2.56 Algorithm'
		rg = ruijie( DEV )
	else:
		print 'we use Xrgsu 1.1.1 Algorithm'
		rg =xrgsu( DEV )
	
	print "--> Sent EAPOL Start"
	sdata = inithead+EAPOL(EAPOL_START) + rg.tail()
	sdata = add_zeros (sdata)
	try:
		s.send( sdata )
		printframe( sdata )
	except socket.error:
		s.close()
		if_orig_up = False
		os.system( 'ifconfig %s up' % DEV)
		s=socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.htons(ETHERTYPE_PAE))
		s.bind((DEV, ETHERTYPE_PAE))
		s.send( sdata )
		printframe( sdata )

	s.settimeout(30)
	outtime = 0 # timeout count
	if not rg.get()[0]:
		print "Interface is not up"
		first_time = True
	else:
		print "Interface is up"
		first_time = False
#	first_req = True
	dst = PAE_GROUP_ADDR
	try:
		while 1:
			try:
				p_a = s.recv(1600)
				print 'recieve packet!'
				printframe(p_a)
				if p_a[0:6] != mymac:
					print 'not my packet, discard.'
					continue
				p = p_a[14:]
				vers,type,eapollen  = unpack("!BBH",p[:4])
				if type == EAPOL_EAPPACKET:
					print 'This is a EAPOL.'
					code, id, eaplen = unpack("!BBH", p[4:8])
					outtime = 0
					if code == EAP_SUCCESS:
						print "\n*******************\n* Got EAP Success *\n*******************\n"
						rg.message(p_a)
						ssid = rg.ssid( p_a )
						while 1:
							sdata = ethernet_header(mymac, dst, ETHERTYPE_PAE) +\
									EAPOL(EAPOL_RG,rg.alive( ssid ) )
							sdata = add_zeros (sdata,nums=60)
							s.send( sdata )
							printframe( sdata )
							if first_time:
								while 1:
									if rg.get()[0]:
										print 'Initiate A Second Time Authentication:-)'
										rg.givemd5( '' )
										sdata = ethernet_header(mymac, dst, ETHERTYPE_PAE) +\
												EAPOL(EAPOL_START) + rg.tail()
										sdata = add_zeros (sdata)
										s.send( sdata )
										printframe( sdata )
										first_time = False
										break
									sleep(1)
								break
							sleep(30)
							if not rg.ping():
								print 'Restart Authentication!'
								rg.givemd5( '' )
								sdata = ethernet_header(mymac, dst, ETHERTYPE_PAE) +\
										EAPOL(EAPOL_START) + rg.tail()
								sdata = add_zeros (sdata)
								s.send( sdata )
								printframe( sdata )
								break
							else:
								sleep(20)
									 
					elif code == EAP_FAILURE:
						print "Got EAP Failure"
						rg.message(p_a)
					elif code == EAP_RESPONSE:
						print "?? Got EAP Response"
					elif code == EAP_REQUEST:
						#						if options.xrgsu and first_req:
#							first_req = False
#							continue
						dst = p_a[6:12]
						reqtype = unpack("!B", p[8:9])[0]
						reqdata = p[9:4+eaplen]
						if reqtype == EAP_TYPE_ID:
							print "Got EAP Request for identity"
							sdata = ethernet_header(mymac, dst, ETHERTYPE_PAE) +\
									EAPOL(EAPOL_EAPPACKET, EAP(EAP_RESPONSE, id,reqtype, USER) )\
									+ rg.tail()
							sdata = add_zeros (sdata)
							s.send( sdata )
							print "--> Sent EAP response with identity = [%s]" % USER
							printframe( sdata )
						elif reqtype == EAP_TYPE_MD5:
							print "Got EAP Request for MD5 challenge"
							challenge=pack("!B",id)+PASS+reqdata[1:17] # [0:1] is the length of data value
							rg.givemd5( reqdata[1:17] )
							resp=md5.md5(challenge).digest()
							if options.md5o:
								# seems we shouldn't add the length of the md3sum in some school.
								resp= resp + USER # add USER for RG
							else:
								resp=chr(len(resp)) + resp + USER # add USER for RG
							sdata = ethernet_header(mymac, dst, ETHERTYPE_PAE) +\
									EAPOL(EAPOL_EAPPACKET, EAP(EAP_RESPONSE, id, reqtype, resp ) )\
									+ rg.tail()
							sdata = add_zeros (sdata)
							s.send( sdata )
							print "--> Send EAP response with MD5 challenge"
							printframe( sdata )
						else:
							print "?? Got unknown Request type (%i)" % reqtype
					else:
						print "?? Got unknown EAP code (%i)" % code
				else:
					print "Got EAPOL type %i" % type
			
			except socket.timeout:
				print 'Timed out! restart'
				if outtime % 2 == 0 and outtime < 7:
					sdata = inithead + EAPOL(EAPOL_START) + rg.tail()
				elif outtime % 2 != 0 and outtime < 7:
					sdata = inithead_2 + EAPOL(EAPOL_START) + rg.tail()
				else:
					print 'Time out too many times! Exit'
					sys.exit(2)
				sdata = add_zeros (sdata)
				s.send( sdata )
				printframe( sdata )
				outtime += 1

	except KeyboardInterrupt:
		print "Interrupted by user"
		rg.givemd5( '' )
		sdata = ethernet_header(mymac, dst, ETHERTYPE_PAE) +\
				EAPOL(EAPOL_LOGOFF) + rg.tail()
		sdata = add_zeros (sdata)
		s.send( sdata )
		printframe( sdata )
		if not if_orig_up:
			os.system( 'ifconfig %s down' % DEV )
		
### the running part
if __name__ == '__main__':
	if os.getuid() != 0:
		print 'Please run this program as Root!'
		sys.exit(1)
	if not options.action:
		startstop('stop',pidfile)
	if options.daemon:
		startstop('start',pidfile,logfile,logfile,'/dev/null')
	main()

### EOF ###
