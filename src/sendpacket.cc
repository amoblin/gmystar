/**************************************************
 *  sendpacket.cc
 *
 *  Copyright (C) 2009  csip(amoblin@gmail.com)
 *
 *  ChangeLog:
 *
 *  Description:
 *	Warning: this file should be in UTF-8.
 *  
 **************************************************/
#include "Mystar.h"
int Mystar::SendFindServerPacket (libnet_t * l)
{
    /* 发起认证   EAPOL-Start
     * 0x00-0x05    目的MAC  6
     * 0x06-0x0b    源MAC    6
     * 0x0c-0x0d    协议类型 2   888E 代表802.1x
     * 0x0e         协议版本号   1
     * 0x0f         帧类型       1   EAPOL-Start的为01
     * 0x10-0x11    Packet Body Length   2  EAPOL-Start为0000
     * */
    static uint8_t broadPackage[0x3E8] =    //广播包，用于寻找服务器  MAC帧
    {	
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x8E, 0x01, 0x01,
/*目的mac                          源mac                               802.1x      版本 类型*/
0x00,0x00, 0xFF, 0xFF, 0x37, 0x77, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*包长度   包数据....                                                                       */
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x15, 0x00, 0x00, 0x13, 0x11, 0x38, 0x30, 0x32,
0x31,0x78, 0x2E, 0x65, 0x78, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*                                                                     锐捷版本号           */
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1F, 0x00,
0x00,0x00, 0x00, 0x00, 0x13, 0x11, 0x00, 0x28, 0x1A, 0x28, 0x00, 0x00, 0x13, 0x11, 0x17, 0x22,
0x92,0x68, 0x64, 0x66, 0x92, 0x94, 0x62, 0x66, 0x91, 0x93, 0x95, 0x62, 0x93, 0x93, 0x91, 0x94,
0x64,0x61, 0x64, 0x64, 0x65, 0x66, 0x68, 0x94, 0x98, 0xA7, 0x61, 0x67, 0x65, 0x67, 0x9C, 0x6B
/*                                                                      校验码              */
    };

    uint8_t StandardAddr[] = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x03 };//802.1标准组播地址

    uint8_t StarAddr[] = { 0x01, 0xD0, 0xF8, 0x00, 0x00, 0x03 };//私有组播地址
    //extern uint8_t m_localMAC[6];
    //extern int m_authenticationMode;
    /*作为参数传入*/
    printf("hello\n");
    if (authenticationMode == 1)    //实达
        memcpy(broadPackage, StarAddr, 6);
    else        //标准
        memcpy(broadPackage, StandardAddr, 6);
    memcpy (broadPackage + 6, localMAC, 6);	//填充MAC地址
    FillNetParamater(&broadPackage[0x17]);
    fputs (">> Searching for server...\n", stdout);
    return (libnet_write_link(l, broadPackage, 0x3E8) == 0x3E8) ? 0 : -1;
}
int Mystar::SendNamePacket (libnet_t * l, const u_char * pkt_data)
{
    /*
     * 发送用户名 EAP-Response/Identity
     * 0x00-0x05    目的MAC             6
     * 0x06-0x0b    源MAC               6
     * 0x0c-0x0d    协议类型            2   888E 代表802.1x
     * 0x0e         协议版本号          1
     * 0x0f         帧类型              1   EAP-Response/Identity的为00
     * 0x10-0x11    Packet Body Length  2   从0x12开始到用户名最后一个字符的长度，网络字节序
     * 0x12                             1   02代表Response 
     * 0x13         Identity            1   填入服务器包中0x13的值  link1
     * 0x14-0x15    重复0x10-0x11       2
     * 0x16         发送用户名          1
     * 0x17开始     用户名              x   
     * 附加包
     * */
    static uint8_t ackPackage[0x3E8] =	//应答包，包括用户名和MD5
    {
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x8E, 0x01, 0x00,
/*目的mac                          源mac                               802.1x协议  版本 类型*/
0x00,0x0D, 0x02, 0x01, 0x00, 0x0D, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
/*长度---  2代表应答   长度------  用户                                        */
0xFF,0x37, 0x77, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x11, 0x38, 0x30, 0x32, 0x31, 0x78, 0x2E,
0x65,0x78, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t ackShida[] =	//实达专有响应附加包
    {
0xFF,0xFF, 0x37, 0x77, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x08, 0x15, 0x00, 0x00, 0x13, 0x11, 0x38, 0x30, 0x32, 0x31, 0x78,
0x2E,0x65, 0x78, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1F, 0x00, 0x00, 0x00,
0x00,0x00, 0x13, 0x11, 0x00, 0x28, 0x1A, 0x28, 0x00, 0x00, 0x13, 0x11, 0x17, 0x22, 0x92, 0x68,
0x64,0x66, 0x92, 0x94, 0x62, 0x66, 0x91, 0x93, 0x95, 0x62, 0x93, 0x93, 0x91, 0x94, 0x64, 0x61,
0x64,0x64, 0x65, 0x66, 0x68, 0x94, 0x98, 0xA7, 0x61, 0x67, 0x65, 0x67, 0x9C, 0x6B
    };

    memcpy (ackPackage, destMAC, 6);	//将目的MAC地址填入组织回复的包
    memcpy (ackPackage + 6, localMAC, 6);	//将本机MAC地址填入组织回复的包

    ackPackage[0x12] = 0x02;	//code,2代表应答
    ackPackage[0x13] = pkt_data[0x13];	//id, HERE as if it's alway 1 from ShiDa ??

    int nameLen;
    nameLen = strlen (user.get_username());

    *(short *) (ackPackage + 0x10) = htons ((short) (5 + nameLen));	//len
    *(short *) (ackPackage + 0x14) = *(short *) (ackPackage + 0x10);	//len
    memcpy (ackPackage + 0x17, user.get_username(), nameLen);	//填入用户名

    FillNetParamater (&ackShida[0x05]);
    memcpy (ackPackage + 0x17 + nameLen, ackShida, 0x6e);//实达附加包

    fputs (">> Sending user name...\n", stdout);

    return (libnet_write_link (l, ackPackage, 0x3E8) == 0x3E8) ? 0 : -1;
}
int Mystar::SendPasswordPacket (libnet_t * l, const u_char * pkt_data)
{
    /*
     * 发送密码   EAP-Response
     * 0x00-0x05    目的MAC             6
     * 0x06-0x0b    源MAC               6
     * 0x0c-0x0d    协议类型            2   888E 代表802.1x
     * 0x0e         协议版本号          1
     * 0x0f         帧类型              1   EAP-Response的为00
     * 0x10-0x11    Packet Body Length  2   从0x12开始到用户名最后一个字符的长度，网络字节序
     * 0x12                             1   02代表Response 
     * 0x13         Identity            1   服务器包中0x13的值
     * 0x14-0x15    重复0x10-0x11       2
     * 0x16         发送用户名和密码    1
     * 0x18-0x27    加密密码            16
     * 0x28开始     用户名              x  
     * 附加包
     * */
    static uint8_t ackPackage[0x3E8] =	//应答包，包括用户名和MD5
    {
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x8E, 0x01, 0x00,
/*目的mac                          源mac                               */
0x00,0x0D, 0x02, 0x01, 0x00, 0x0D, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
/*         2代表应答                           密码******************************************/
0xFF,0x37, 0x77, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/********************************************  用户名                      */
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x11, 0x38, 0x30, 0x32, 0x31, 0x78, 0x2E,
0x65,0x78, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t ackShida[] =	//实达专有响应附加包
    {
0xFF,0xFF, 0x37, 0x77, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x08, 0x15, 0x00, 0x00, 0x13, 0x11, 0x38, 0x30, 0x32, 0x31, 0x78,
0x2E,0x65, 0x78, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1F, 0x00, 0x00, 0x00,
0x00,0x00, 0x13, 0x11, 0x00, 0x28, 0x1A, 0x28, 0x00, 0x00, 0x13, 0x11, 0x17, 0x22, 0x92, 0x68,
0x64,0x66, 0x92, 0x94, 0x62, 0x66, 0x91, 0x93, 0x95, 0x62, 0x93, 0x93, 0x91, 0x94, 0x64, 0x61,
0x64,0x64, 0x65, 0x66, 0x68, 0x94, 0x98, 0xA7, 0x61, 0x67, 0x65, 0x67, 0x9C, 0x6B
    };

    md5.md5Len=0;

    int nameLen, passwordLen;

    nameLen = strlen (user.get_username());
    passwordLen = strlen (user.get_password());

    memcpy (ackPackage, destMAC, 6);
    memcpy (ackPackage + 6, localMAC, 6);	//将本机MAC地址填入组织回复的包

    ackPackage[0x12] = 0x02;	//code,2代表应答
    ackPackage[0x13] = pkt_data[0x13];	//id
    *(ackPackage + 0x16) = *(pkt_data + 0x16);	//type，即应答方式,HERE should alway be 4

    *(short *) (ackPackage + 0x10) = htons ((short) (22 + nameLen));	//len
    *(short *) (ackPackage + 0x14) = *(short *) (ackPackage + 0x10);

    md5.md5Data[md5.md5Len++] = ackPackage[0x13];	//ID
    memcpy (md5.md5Data + md5.md5Len, user.get_password(), passwordLen);
    md5.md5Len += passwordLen;	//密码
    memcpy (md5.md5Data + md5.md5Len, pkt_data + 0x18, pkt_data[0x17]);
    md5.md5Len += pkt_data[0x17];	//密匙
    md5.md5Dig = md5.ComputeHash(md5.md5Data, md5.md5Len);

    ackPackage[0x17] = 16;	//length of md5sum is always 16.
    memcpy (ackPackage + 0x18, md5.md5Dig, 16);

    memcpy (ackPackage + 0x28, user.get_username(), nameLen);

    FillNetParamater (&ackShida[0x05]);
    memcpy (ackPackage + 0x28 + nameLen, ackShida, 0x6e);//实达附加包

    fputs (">> Sending password... \n", stdout);
    return (libnet_write_link (l, ackPackage, 0x3E8) == 0x3E8) ? 0 : -1;
}
int Mystar::SendEchoPacket(libnet_t * l, const u_char * pkt_data, ULONG_BYTEARRAY *m_serialNo)
{
    /*  发送激活包
     * 0x00-0x05    目的MAC             6
     * 0x06-0x0b    源MAC               6
     * 0x0c-0x0d    协议类型            2   888E 代表802.1x
     * 0x0e         协议版本号          1
     * 0x0f         帧类型              1   这里为bf
     * 0x10-0x11    Packet Body Length  2   从0x12开始到用户名最后一个字符的长度，网络字节序
     * 0x12-0x17    ff ff 37 77 7f 9f   6
     * 0x18-0x1b    认证成功时收到的
     * 0x1c-0x1f    ff ff 37 77         4
     * 0x22-0x25    序列号转化
     * 0x26-0x2c    ff ff 37 77 7f 3f ff    7
     * */
    static uint8_t echoPackage[] =	//echo包，用于每5秒钟激活一次
    {
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x8E, 0x01, 0xBF,
/*目的mac                          源mac                               其他*/
0x00,0x1E, 0xFF, 0xFF, 0x37, 0x77, 0x7F, 0x9F, 0xF7, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x37, 0x77,
/*                                             alog0 alog1 alog2 alog3     */
0x7F,0x9F, 0xF7, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x37, 0x77, 0x7F, 0x3F, 0xFF
/*         alog0 */
    };
    ULONG_BYTEARRAY uCrypt1, uCrypt2, uCrypt1_After, uCrypt2_After;

    m_serialNo->ulValue++;

    uCrypt1.ulValue = m_key.ulValue + m_serialNo->ulValue;
    uCrypt2.ulValue = m_serialNo->ulValue;

    memcpy (echoPackage, destMAC, 6);
    memcpy (echoPackage + 6, localMAC, 6);

    uCrypt1_After.ulValue = htonl (uCrypt1.ulValue);
    uCrypt2_After.ulValue = htonl (uCrypt2.ulValue);

    echoPackage[0x18] = Alog (uCrypt1_After.btValue[0]);
    echoPackage[0x19] = Alog (uCrypt1_After.btValue[1]);
    echoPackage[0x1a] = Alog (uCrypt1_After.btValue[2]);
    echoPackage[0x1b] = Alog (uCrypt1_After.btValue[3]);
    echoPackage[0x22] = Alog (uCrypt2_After.btValue[0]);
    echoPackage[0x23] = Alog (uCrypt2_After.btValue[1]);
    echoPackage[0x24] = Alog (uCrypt2_After.btValue[2]);
    echoPackage[0x25] = Alog (uCrypt2_After.btValue[3]);
#ifdef DEBUG
    //printf("send a echo.\n");
#endif
    return (libnet_write_link(l, echoPackage, 0x2d) == 0x2d) ? 0 : -1;
    //TODO:0x2d的含义？
}
int Mystar::SendEndCertPacket (libnet_t * l)//结束通信
{
    /* 断开连接 EAP-Logoff
     * 0x00-0x05    目的MAC             6
     * 0x06-0x0b    源MAC               6
     * 0x0c-0x0d    协议类型            2   888E 代表802.1x
     * 0x0e         协议版本号          1
     * 0x0f         帧类型              1   EAP-Logoff的为02
     * 0x10-0x11    Packet Body Length  2   EAP-Logoff为0000
     * */
    static uint8_t ExitPacket[] =
    {
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x8E, 0x01, 0x02,
/*目的mac                          源mac                               802.1x               */
0x00,0x00, 0xFF, 0xFF, 0x37, 0x77, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x15, 0x00, 0x00, 0x13, 0x11, 0x38, 0x30, 0x32,
0x31,0x78, 0x2E, 0x65, 0x78, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1F, 0x00,
0x00,0x01, 0x00, 0x00, 0x13, 0x11, 0x00, 0x28, 0x1A, 0x28, 0x00, 0x00, 0x13, 0x11, 0x17, 0x22,
0x64,0x91, 0x60, 0x60, 0x65, 0x65, 0x69, 0x61, 0x64, 0x64, 0x94, 0x93, 0x91, 0x92, 0x96, 0x65,
0x95,0x64, 0x68, 0x91, 0x62, 0x68, 0x62, 0x94, 0x9A, 0xD6, 0x94, 0x68, 0x66, 0x69, 0x6C, 0x65
    };

    memcpy(ExitPacket, destMAC, 6);
    memcpy(ExitPacket + 6, localMAC, 6);
    FillNetParamater (&ExitPacket[0x17]);
    cout<<">> Logouting... \n"<<endl;
    return (libnet_write_link (l, ExitPacket, 0x80) == 0x80) ? 0 : -1;
}
