/***************************************************************************
 *   Copyright (C) 2008 by duanp   *
 *   duanp@163.com   *
*     based  on  NetXRay  's  version  "mystar"
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 

#ifndef MYSTAR_H
#define MYSTAR_H

#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <string.h>
//#include <signal.h>
//#include <errno.h>
//#include <setjmp.h>
//#include <time.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/ioctl.h>

//#include <arpa/inet.h>

#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>

#include <pcap/pcap.h>
#include <libnet.h>

#include <qtextedit.h>

typedef union
{
	u_int32_t   ulValue;
	u_int8_t    btValue[4];
}  Union32;

typedef struct 
{
	char *  userName;
	char *  passWord;
	int	  authMode;
	char *  nic;
	int 	  echoInterval;
	bool   isIntelligentReconnect;
	bool   isRemember;
	bool   isFakeInUse;
	bool   isAutoRun;
	bool   isAutoConnect;
	bool   isAutoMinimize;
	unsigned char  fakeAddress[4];  
	

} StarConfig;

enum AuthState {
	before_init,
	init_ok,
	server_ok,//----|
	name_ok,//	|
	pass_ok  ,//	|
	auth_ok ,  auth_error
	
};

class StarAuth {

public :		
	StarAuth(StarConfig * ,QTextEdit *);
	
	StarAuth();
	~StarAuth();	

	AuthState   authState;
	
	void Run();
	void Init();	
	void RunEcho();
	void RunBye();
	void SetConfig(StarConfig * conf);
	void SetOutPut(QTextEdit * qtext);
	void OutPut(char *);
	int SendFindServerPacket(libnet_t *l);
	int SendNamePacket(libnet_t *l, const unsigned char  *pkt_data);
	int SendPasswordPacket(libnet_t *l,const unsigned char *pkt_data);
	int SendEchoPacket(libnet_t *l);
	int SendEndCertPacket(libnet_t *l);

private:
	StarConfig *  config;

	QTextEdit * TextEdit;

	unsigned char  localMac[6];
	unsigned char  destMac[6];
	unsigned char  localIP[4];
	unsigned char  localNetMask[4];
	unsigned char  localGetway[4];
	unsigned char  localDns[4];	
	unsigned char  circleCheck[2];

	Union32  serialNo;
	Union32  key;

	libnet_t  * ptrLibnet;
	pcap_t             * ptrPcap;
	int p_fd;
	
	void      CircleCheckGen();
	unsigned char ByteTransform(unsigned char);	
	void FillNetParamater(unsigned char *);


};



#endif /* MYSTAR_H */
