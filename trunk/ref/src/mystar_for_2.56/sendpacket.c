/*
    本文件改编自MentoSupplicant3.8,几乎是照着抄过来的 :)
    (其实也有一点点差别了,如果你对照着二者看的话...)
    我觉得这个文件中有很多代码可以合并优化,比如那个ackShiDa在好几个函数里都
出现过. 但我恐怕不想去完善了,主要的原因是几乎没人用,没热情......
     希望有兄弟接着完善它.
     
     netxray@byhh 2004/9/7 15:20
*/
/*Last modified by ipconfig(http://cudev.cublog.cn) 2006/11/29*/

#include "sendpacket.h"
#include "global.h"
#include "blog.h"
#include "md5.h"

uint8_t ackShida[] = { 
        //1-->22
        0xFF,0xFF,0x37,0x77,0xFF,
        0x00,0x00,0x00,0x00,            // Encode( IP )
        0x00,0x00,0x00,0x00,            // Encode( SubNetMask )
        0x00,0x00,0x00,0x00,            // Encode( NetGate )
        //在2.56以后可能会有改变
        0x00,0x00,0x00,0x00,            // Encode( DNS )
        0x00,0x00,                      // Checksum( )
        // 23 --> 58
        0x00,0x00,0x13,0x11,0x38,0x30,0x32,0x31,0x78,0x2E,0x65,0x78,0x65,0x00,0x00,0x00,    // ASCII 8021x.exe
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    //
        0x00,0x00,0x00,0x00,
        // 59 --> 62
        //0x02,0x32,0x00,0x00,            // 8021x.exe File Version (2.5.00) 
        0x02,0x38,0x00,0x00,            // 8021x.exe File Version (2.56.00) 
        //63
        0x00,                           // unknow flag(有时为0；有时为1)
        //64-->77
        0x00,0x00,0x13,0x11,0x00,0x28,0x1A,0x28,0x00,0x00,0x13,0x11,0x17,0x22,	            // Const strings
        // 78 --> 118
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,	// 32 bits spc. Random strings or MD5 Hash
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,    // 32 bits spc. Random strings or MD5 Hash
	//110-120
        0x1a,0x0c,0x00,0x00,0x13,0x11,0x18,0x06,0x00,0x00,0x00,       // Const strings
        //老版本的瑞捷到此为止
        // 121
        0x00,                           // DHCP and first time flag
       // V2.56 (and upper?) added 
       // 122 -->129
        0x1A,0x0E,0x00,0x00,0x13,0x11,0x2D,0x08,                     // Const strings
        // 130 --> 143
        0x00,0x00,0x00,0x00,0x00,0x00,                               // True NIC MAC
        0x1A,0x08,0x00,0x00,0x13,0x11,0x2F,0x02                      // Const strings
        };
        
int SendFindServerPacket(libnet_t *l)
{
    static uint8_t broadPackage[0x3E8] = {      //广播包，用于寻找服务器
	           //0x00-0x11
                    0x00,0x00,0x00,0x00,0x00,0x00,// Destination MAC
                    0x00,0x00,0x00,0x00,0x00,0x00,// Source MAC
                    0x88,0x8E,// Ethertype = 0x888E (8021X)
                    0x01,// Version = 1
                    0x01,// Packet Type  0x00 ;0x01,EAPOL-Start ;0x02 ;0x03 ;0x04
                    0x00,0x00,// Packet Body Length
                   };

    uint8_t StandardAddr[] = {0x01,0x80,0xC2,0x00,0x00,0x03}; //标准组播地址
    uint8_t StarAddr[]     = {0x01,0xD0,0xF8,0x00,0x00,0x03}; //实达私有组播地址

    extern uint8_t  m_localMAC[6];
    extern int    m_authenticationMode;

    if (m_authenticationMode==1)
        memcpy(broadPackage,StarAddr,6);
    else
        memcpy(broadPackage,StandardAddr,6);
    memcpy( broadPackage+6, m_localMAC, 6 );   //填充MAC地址
    
    FillNetParamater( &ackShida[0x05] );
    memcpy( ackShida+130, m_localMAC, 6 );
    memcpy( broadPackage+0x17,ackShida,0x90);

    fputs(">> Searching for server...\n",stdout);

    return (libnet_write_link(l,broadPackage, 0x3E8)==0x3E8)?0:-1;
}

int SendNamePacket(libnet_t *l, const u_char *pkt_data)
{
    static uint8_t ackPackage[0x3E8] = {     //应答包，包括用户名
	           //0x00-0x11
                    0x00,0x00,0x00,0x00,0x00,0x00,// Destination MAC
                    0x00,0x00,0x00,0x00,0x00,0x00,// Source MAC
                    0x88,0x8E,// Ethertype = 0x888E (8021X)
                    0x01,// Version = 1
                    0x01,// Packet Type  0x00 ;0x01,EAPOL-Start ;0x02 ;0x03 ;0x04
                    0x00,0x00,// Packet Body Length
		   //0x12
		    0x02, //表示应答
		   //0x13
		    0x01, //ID
		   //0x14-0x15
		    0x00,0x00,	//Packet Body Length(同上面的0x10-0x11)
		   //0x16
		    0x01,	//Identify(RFC3748)
		   //0x17  用户名（视情况而定）
		   //0x17+NameLen  同前面FindServer的数据包;那个随机的字符串是一样的
		    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    		};
  
    extern char *m_name;
    extern uint8_t  m_destMAC[6];
    extern uint8_t  m_localMAC[6];
    int nameLen;

    nameLen=strlen(m_name);
    memcpy(ackPackage,m_destMAC,6);  //将目的MAC地址填入组织回复的包
    memcpy(ackPackage+6,m_localMAC,6);  //将本机MAC地址填入组织回复的包
    ackPackage[0x12]=0x02;            //code,2代表应答
    ackPackage[0x13]=pkt_data[0x13];  //id, HERE as if it's alway 1 from ShiDa ??
    *(short *)(ackPackage+0x10) = htons((short)(5+nameLen));//len
    *(short *)(ackPackage+0x14) = *(short *)(ackPackage+0x10);//len
    memcpy(ackPackage+0x17,m_name,nameLen); //填入用户名

    FillNetParamater( &ackShida[0x05] );
    memcpy( ackShida+130, m_localMAC, 6 );
    memcpy(ackPackage+0x17+nameLen,ackShida,0x90);

    fputs(">> Sending user name...\n",stdout);

    return (libnet_write_link(l,ackPackage, 0x3E8)==0x3E8)?0:-1;
}

int SendPasswordPacket(libnet_t *l,const u_char *pkt_data)
{
	    static uint8_t ackPackage[0x3E8] = {        //应答包，包括用户名和MD5
	                   //0x00-0x11
                            0x00,0x00,0x00,0x00,0x00,0x00,// Destination MAC
                            0x00,0x00,0x00,0x00,0x00,0x00,// Source MAC
                            0x88,0x8E,// Ethertype = 0x888E (8021X)
                            0x01,// Version = 1
                            0x00,// Packet Type  0x00 ;0x01,EAPOL-Start ;0x02 ;0x03 ;0x04
                            0x00,0x00,// Packet Body Length
			   //0x12
			    0x02, //表示应答
			   //0x13
			    0x00, //ID(根据前面的服务器响应进行设置)
			   //0x14-0x15
			    0x00,0x00,	//Packet Body Length(同上面的0x10-0x11)
			   //0x16
			    0x04,	//Type(MD5-Challenge)
			   //0x17  
			    0x00,        //Value-size(一般为16)
			   //0x18-(0x18+Value-size-1) 
			   //散列序列
		           //注：散列序列的形成:   Hash(ID || Password || ReciveKey)
          		   // 其中ID、ReciveKey均是通过上一次会话从服务器获得的
			   //（0x18+Value-size）-(0x18+Value-size + NameLen-1)
	        	   //用户名
			   //(0x18+Value-size + NameLen)-
			   //同前面FindServer的数据包;那个随机的字符串是一样的 
			   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    			};
 
    unsigned char   md5Data[256]; //密码,md5 buffer
    unsigned char  *md5Dig;       //result of md5 sum
    int             md5Len=0;

    extern char *m_name;
    extern char *m_password;
    extern uint8_t  m_destMAC[6];
    extern uint8_t  m_localMAC[6];
    int nameLen,passwordLen;

    nameLen=strlen(m_name); passwordLen=strlen(m_password);

    memcpy(ackPackage,m_destMAC,6);
    memcpy(ackPackage+6,m_localMAC,6); //将本机MAC地址填入组织回复的包

    ackPackage[0x12] = 0x02;                //code,2代表应答
    ackPackage[0x13]=pkt_data[0x13];        //id
    *(ackPackage+0x16) = *(pkt_data+0x16);  //type，即应答方式,HERE should alway be 4

    *(short *)(ackPackage+0x10) = htons((short)( 22+nameLen)); //len
    *(short *)(ackPackage+0x14) = *(short *)( ackPackage+0x10 );

    md5Data[md5Len++] = ackPackage[0x13];//ID
    memcpy(md5Data+md5Len,m_password,passwordLen); md5Len+=passwordLen; //密码
    memcpy(md5Data+md5Len,pkt_data+0x18,pkt_data[0x17]); md5Len+=pkt_data[0x17]; //密匙
    md5Dig =ComputeHash( md5Data, md5Len);

    ackPackage[0x17]=16;               //length of md5sum is always 16.
    memcpy(ackPackage+0x18,md5Dig,16);

    memcpy(ackPackage+0x28,m_name,nameLen);

    FillNetParamater( &ackShida[0x05] );
    memcpy( ackShida+130, m_localMAC, 6 );
    memcpy(ackPackage+0x28+nameLen,ackShida,0x90);

    fputs(">> Sending password... \n",stdout);
    return (libnet_write_link(l,ackPackage, 0x3E8)==0x3E8)?0:-1;
}

int SendEchoPacket(libnet_t *l,const u_char *pkt_data)
{
    static uint8_t echoPackage[] ={	//echo包，用于每n秒钟激活一次
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x8E,0x01,0xBF,
		0x00,0x1E,0xFF,0xFF,0x37,0x77,0x7F,0x9F,0xF7,0xFF,0x00,0x00,0xFF,0xFF,0x37,0x77,
		0x7F,0x9F,0xF7,0xFF,0x00,0x00,0xFF,0xFF,0x37,0x77,0x7F,0x3F,0xFF};
    ULONG_BYTEARRAY uCrypt1,uCrypt2,uCrypt1_After,uCrypt2_After;
    extern ULONG_BYTEARRAY  m_serialNo;
    extern ULONG_BYTEARRAY  m_key;
    extern uint8_t  m_destMAC[6];
    extern uint8_t  m_localMAC[6];

    m_serialNo.ulValue++;
    //m_serialNo is initialized at the beginning of main() of mystar.c, and
    //m_key is initialized in mystar.c when the 1st Authentication-Success packet is received.

    uCrypt1.ulValue = m_key.ulValue + m_serialNo.ulValue;
    uCrypt2.ulValue = m_serialNo.ulValue;

    memcpy( echoPackage, m_destMAC, 6 );
    memcpy( echoPackage+6, m_localMAC, 6 );

    uCrypt1_After.ulValue = htonl( uCrypt1.ulValue );
    uCrypt2_After.ulValue = htonl( uCrypt2.ulValue );

    echoPackage[0x18] = Alog(uCrypt1_After.btValue[0]);
    echoPackage[0x19] = Alog(uCrypt1_After.btValue[1]);
    echoPackage[0x1a] = Alog(uCrypt1_After.btValue[2]);
    echoPackage[0x1b] = Alog(uCrypt1_After.btValue[3]);
    echoPackage[0x22] = Alog(uCrypt2_After.btValue[0]);
    echoPackage[0x23] = Alog(uCrypt2_After.btValue[1]);
    echoPackage[0x24] = Alog(uCrypt2_After.btValue[2]);
    echoPackage[0x25] = Alog(uCrypt2_After.btValue[3]);

    return (libnet_write_link(l,echoPackage, 0x2d)==0x2d)?0:-1;
}

int  SendEndCertPacket(libnet_t *l)
{
    extern uint8_t  m_destMAC[6];
    extern uint8_t  m_localMAC[6];

    static uint8_t ExitPacket[]={  //edited by soar
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x8E,0x01,0x02, 
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x15,0x00,0x00,0x00,0x00,0x00
		};//edited by soar

    memcpy( ExitPacket, m_destMAC, 6 );
    memcpy( ExitPacket+6,m_localMAC, 6 );
    FillNetParamater( &ExitPacket[0x17] );
    memcpy( ackShida+130, m_localMAC, 6 );
    memcpy(ExitPacket+0x12,ackShida,0x90);
    
    fputs(">> Logouting... \n",stdout);
    return (libnet_write_link(l,ExitPacket,0x3E8)==0x3E8)?0:-1;
}
