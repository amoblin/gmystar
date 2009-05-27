#!/bin/bash
# This is the install file for XMU-Ruijie
# Author: YuYuwei

inst(){
	install -v -m 755 xmuruijie.py /usr/sbin/xmuruijie
	install -v -m 644 xmuruijie.conf /etc/xmuruijie.conf
	install -v -m 644 8021x.exe /etc/8021x.exe
	install -v -m 755 startscript.sh /etc/init.d/xmuruijie
	echo 
	echo '*******************************************'
	echo '* Remenber to change the user and passwd  *'
	echo '* in /etc/xmuruijie.conf first !   *'
	echo '*******************************************'
	echo 
	echo '*******************************************'
	echo '* /etc/init.d/xmuruijie is the start-stop *'
	echo '* script of xmuruie for non-Gentoo user   *'
	echo '* in DHCP mode, as for Static IP, I only  *'
	echo '* know the method in Gentoo.              *'
	echo '*******************************************'
}
	

uninst(){
	rm -v /usr/sbin/xmuruijie /etc/xmuruijie.conf /etc/8021x.exe /etc/init.d/xmuruijie
}
case $1 in
install)
	inst
	;;
uninstall)
	uninst
	;;
esac
exit 0
