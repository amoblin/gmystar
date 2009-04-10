#include "Mystar.h"
#include "CodeConverter.h"
#define MAXCOUNT 30
//using namespace Mystar;
Mystar::Mystar()
{
    static char file[20];
    strcpy(file,getenv("HOME"));
    strcat(file,"/.gMystarrc");
    filename = file;
    init();//finished!
    checkAndSetConfig();//finished!
}
void Mystar::init(void) 
{
    l = NULL;
    p = NULL;
    isFirstPacketFromServer = 1;
}
void Mystar::authen(int *flag)//原来用while的那个
{
    if(Mystar::init_authen()==0)
        eapol(flag);
}
int Mystar::init_authen()
{
    char l_errbuf[LIBNET_ERRBUF_SIZE];  //错误缓冲区？
    l = libnet_init(LIBNET_LINK, nic, l_errbuf);    //单数据包内存初始化及环境建立
    if(l==NULL)
        err_quit("libnet_init:%s\n", l_errbuf); //网卡初始化错误:找不到网卡或用户权限不够
    p = pcap_open_live(nic, 65536, 0, 500, p_errbuf);   //打开相应网卡并设置非混杂模式
    if(p==NULL)
    {
        err_msg("pcap_open_live:%s\n", p_errbuf);
        //goto err1
        libnet_destroy(l);
        return 1;
    }
    p_fd = pcap_fileno(p);  //返回savefile的表述符号
    l_ether_addr = libnet_get_hwaddr(l);
    if(l_ether_addr == NULL)
    {
        err_msg("unable to get local mac address :%s\n", libnet_geterror(l));
    }
    memcpy(localMAC, l_ether_addr, sizeof(localMAC));
#ifdef DEBUG
    printf("localmac为:");
    int i;
    for(i=0;i<6;i++)
    {
        printf("%2x ",*(localMAC+i));
    }
    printf("\n");
#endif

    if(user.get_fakeAddress()==NULL)
    {
        l_ip=libnet_get_ipaddr4(l);//指向ip的指针
        if(l_ip ==-1)
        {
            err_msg("unable to get ip address--ingored... :%s\n",libnet_geterror(l));
            l_ip=0;
        }
        memcpy(m_ip,&l_ip,sizeof(m_ip));
        printf("现在m_ip为:");
        for(i=0;i<3;i++)
        {
            printf("%d.",*(m_ip+i));
        }
        printf("%d\n",*(m_ip+i));
    }
    else
    {
        if( inet_pton(AF_INET,user.get_fakeAddress(),m_ip)<=0 )
            err_msg("invalid fakeAddress found in mystar.conf, ignored...\n");
        printf("设置m_ip为:");
        for(i=0;i<3;i++)
        {
            printf("%d.",*(m_ip+i));
        }
        printf("%d\n",*(m_ip+i));
    };

    if(pcap_lookupnet(nic,&p_netaddr,&p_netmask,p_errbuf)==-1)
    {
        err_msg("unable to get netmask--igored... %s\n",p_errbuf); p_netmask=0;}
        memcpy(m_netmask,&p_netmask,sizeof(m_netmask));

        InitializeBlog(m_ip,m_netmask,m_netgate,m_dns1); //see blog.c and bloc.h for details

        //set the filter. Here I'm sure filter_buf is big enough.
        snprintf(filter_buf,sizeof(filter_buf),FILTER_STR, localMAC[0],localMAC[1],
                localMAC[2],localMAC[3],localMAC[4],localMAC[5],localMAC[6]);
        if(pcap_compile(p, &filter_code,filter_buf, 0, p_netmask)==-1)
        {
            err_msg("pcap_compile(): %s", pcap_geterr(p));
            //goto err2; 
            pcap_close(p);
            libnet_destroy(l);
            return 1;
        }
        if(pcap_setfilter(p, &filter_code)==-1)
        {
            err_msg("pcap_setfilter(): %s", pcap_geterr(p));
            //goto err2;
            pcap_close(p);
            libnet_destroy(l);
            return 1;
        }
        pcap_freecode(&filter_code); // avoid  memory-leak

        //(void)signal(SIGINT,logout);  //We can exit with Ctrl+C
        (void)sigfillset(&sigset_full);
        (void)sigprocmask(SIG_BLOCK,&sigset_full,NULL); //block all signals.
        return 0;
}
int Mystar::eapol(int *flag) //TODO: 2009-02-23开始
{
    /*
     * 返回值：
     *  0:成功
     *  1:错误  ,则终止程序
     *  2:超时  ,重连，到一阈值停止,这里应监测网口状态，是否连接？
     *  3:认证失败:服务器信息
     * */
#ifdef DEBUG
    printf("执行eapol协议\n");
    printf("先发一个包作开始：\n");
#endif
    m_state = 0;    //disconnecting state
    (void)SendFindServerPacket(l);	//the first time to search for server
    int packetCount_SentFindServer = 1;
    int packetCount_SentName = 0;
    int packetCount_SentPassword = 0;
    int i =1;
    while (*flag)    //线程信号
    {
#ifdef DEBUG
        printf("第%d次循环:\n",i);
#endif
        i++;
        (void) sigfillset(&sigset_full);
        (void) sigdelset(&sigset_full, SIGINT);
        FD_ZERO (&read_set);
        FD_SET (p_fd, &read_set);
        timeout.tv_sec = 1;
        timeout.tv_nsec = 0;	// 1 second

        //wait with all signals(except SIGINT) blocked.
        switch ( pselect(p_fd+1,&read_set,NULL,NULL,&timeout,&sigset_full) )
        {
            case -1: //Normally, this case should not happen since sig_intr() never returns!
                //error;
                pcap_close(p);
                libnet_destroy(l);
                return 1;
            case 0:  //timed out
                switch(m_state)
                {
                    case 0:
                        if(++packetCount_SentFindServer>MAXCOUNT)
                        {
                            puts("search server time out, Restarting authenticaton!");
                            return 2;
                        }
                        (void)SendFindServerPacket(l);
                        continue; //jump to next loop of while(1) to receive next packet
                    case 1:
                        if(++packetCount_SentName>MAXCOUNT)
                        {
                            puts("send username time out, Restarting authenticaton!");
                            return 2;
                        }
                        (void)SendNamePacket(l, pkt_data);
                        continue;
                    case 2:
                        if(++packetCount_SentPassword>MAXCOUNT)
                        {
                            puts("sent password time out, Restarting authenticaton!");
                            return 2;
                        }
                        (void)SendPasswordPacket(l, pkt_data);
                        continue;
                    default:         //error;
                        pcap_close(p);
                        libnet_destroy(l);
                        return 1;
                }
        }

        //Here return value of pselect must be 1

        if((pcap_next_ex(p,&pkt_hdr,&pkt_data))!=1) continue;

        //收到的第二个及其以后的有效packet的源MAC必须等于头次收到的有效分组的源MAC
        if ((!isFirstPacketFromServer)&&(memcmp(destMAC,pkt_data+6,6)!=0)) continue;

        //received a packet successfully. for convenience, SUPPOSE it's the RIGHT packet!! but maybe WRONG!!
        //for example, we have NEVER vefified the length of packet, fancying the packet's length is 0x11 ?!

        switch( pkt_data[0x12] )        //分析EAP包类型
        {
            case 0x01:        //表示请求    2:EAP Request
                switch( pkt_data[0x16] )
                {
                    case 0x01:   //type 1,以用户名应答
                        if(m_state!=0)
                            continue;
                        cout<<"csip\n";
                        show_message("Server found, requesting user name...\n");
                        send_username();    //  3:EAP Response
                        ++packetCount_SentName;
                        break;
                    case 0x04:   //type 4,挑战，以MD5计算得到的值应答   6:Challenge Request
                        if(m_state!=1)
                            continue;
                        show_message("User name valid, requesting password...\n");
                        ++packetCount_SentPassword;
                        send_password();    //  7:EAP Challenge Response
                        break;
                }
                //fputs("回应完请求.\n",stdout);
                break;
            case 0x03:         //认证成功       10:EAP Success
                if(m_state!=2)
                    continue;
                m_state=3;
                show_message("@@ Password valid, authentication SUCCESS!!! :)\n");
                if (echoInterval<=0)  //user has echo disabled
                {
                    //goto done; 
                    printf("echoInterval:%d",echoInterval);
                    pcap_close(p); libnet_destroy(l);
                    return 1;
                }
                //uTemp.ulValue = *(((u_long *)(pkt_data+0x9d)));
                offset=ntohs( *((u_int16_t*)(pkt_data+0x10)) );
                uTemp.ulValue = *((u_int32_t *)(pkt_data+(0x11+offset)-0x08));
                m_key.btValue[0] = Alog(uTemp.btValue[3]);
                m_key.btValue[1] = Alog(uTemp.btValue[2]);
                m_key.btValue[2] = Alog(uTemp.btValue[1]);
                m_key.btValue[3] = Alog(uTemp.btValue[0]);

                //unblock SIGINT, so we can exit with Ctrl+C
                (void)sigemptyset(&sigset_zero);
                (void)sigaddset(&sigset_zero,SIGINT);
                (void)sigprocmask(SIG_UNBLOCK,&sigset_zero,NULL);
                // continue echoing
                show_message("Keeping sending echo...\n");

                ULONG_BYTEARRAY m_serialNo;
                m_serialNo.ulValue = 0x1000002a;//the initial serial number, a magic number!
                while(*flag)
                {
                    if(SendEchoPacket(l, pkt_data, &m_serialNo)!=0)
                        break;
                    sleep(echoInterval);
                }
                //goto err2; //this should never happen.
                pcap_close(p);
                libnet_destroy(l);
                return 1;
                break;
            case 0x04:        //认证失败(用户名或密码错误/不在上网时段内/重复上网等)    16
                if((m_state==0)||(m_state==3)) continue;
                m_state=0;
                msgLen=ntohs( *((u_int16_t*)(pkt_data+0x10)) )-10;
                char *msgBuf_utf8=NULL;
                if (msgLen>0)
                {
                    if(msgLen>=(sizeof(msgBuf)-1)) msgLen=sizeof(msgBuf)-1;
                    memset(msgBuf,'\0',sizeof(msgBuf));
                    memcpy(msgBuf, pkt_data+0x1c, msgLen);
                    pmsgBuf=msgBuf;
                    //remove the leanding "\r\n" which seems alway to exist!
                    if((msgLen>2)&&(msgBuf[0]==0xd)&&(msgBuf[1]==0xa)) pmsgBuf+=2;

#ifdef DEBUG
                    /*
                       char *msgBuf_p=msgBuf;
                       printf("服务器返回的信息(十六进制码格式)为：");
                       while(*msgBuf_p)
                       {

                       printf("0x%2x,",(unsigned)*msgBuf_p%256);
                       msgBuf_p++;
                       }
                       printf("\n");
                       printf("服务器返回的信息(GBK编码格式)为：%s\n",msgBuf);
                     */
#endif
                    /*
                     *{ 关于编码的注释
                     * GBK编码，中文两字节，英文一字节
                     * 根据每字节首位来判断:
                     * 若为0,则表示余下七位表示ASCII字符
                     * 若为1,则表示接下来的两个字节表示一个汉字
                     *
                     * 0    0000
                     * 1    0001
                     * 2    0010
                     * 3    0011
                     * 4    0100
                     * 5    0101
                     * 6    0110
                     * 7    0111
                     * 8    1000
                     * 9    1001
                     * a    1010
                     * b    1011
                     * c    1100
                     * d    1101
                     * e    1110
                     * f    1111
                     *
                     *0x d,0x a,0x49,0x50,0xb5,0xd8,0xd6,0xb7,
                     *  \n   \r   I    P      地        址        
                     *
                     * \n   0000 1101
                     * \r   0000 1010
                     * I    0100 1001
                     * P    0101 0000
                     * 地   1011 0101 1101 1000
                     * 址   1101 0110 1011 0111
                     *
                     *
                     *0xb0,0xf3,0xb6,0xa8,0xb4,0xed,0xce,0xf3,0x21
                     *   绑        定        错        误     !
                     *0x d,0x a,0xd2,0xd1,0xb4,0xef,0xb5,0xbd,0xcd,0xac,0xca,0xb1,0xd4,0xda,
                     *              已        达        到        同        时        在
                     0xcf,0xdf,0xd3,0xc3,0xbb,0xa7,0xca,0xfd,0xc1,0xbf,0xc9,0xcf,0xcf,0xde,0x21
                     *  线        用        户        数        量        上        限      !
                     *0x d,0x a,0xd3,0xc3,0xbb,0xa7,0xb2,0xbb,0xb4,0xe6,0xd4,0xda,
                     *              用        户        不        存        在
                     *0xbb,0xf2,0xc3,0xdc,0xc2,0xeb,0xb4,0xed,0xce,0xf3,0x21,
                     *    或        密       码         错        误      !
                     *0x d,0x a,0xb2,0xbb,0xd4,0xda,0xc8,0xcf,0xd6,0xa4,
                     *              不        在        认        证
                     0xca,0xb1,0xb6,0xce,0xc4,0xda,0x21
                     *   时        段        内      !
                     * 
                     *}*/
                    msgBuf_utf8=gbk2utf8(msgBuf);//转换成功，但显示不出来。。。
                    //ConvertGBKToUtf8(msgBuf_utf8,msgBuf);

                    // gb2312-->utf-8
                    //CodeConverter cc2 = CodeConverter("gb2312","utf-8");//这两行出现段错误
                    //cc2.convert(msgBuf,strlen(msgBuf),msgBuf_utf8,255);

                }
                else //这个估计是服务器告知静默或确认断网的包
                {
                    pmsgBuf=NULL;
                }
                //fprintf(stdout,"@@ Authenticaton failed!!! :(%s\n",pmsgBuf);
                //fprintf(stdout,"@@ Authenticaton failed!!! :(%s\n",msgBuf_utf8);
                printf("@@ Authenticaton failed!!! :(%s\n",msgBuf_utf8);
#ifdef DEBUG
                //printf("认证失败，返回信息转化为UTF8编码后为：%s\n",msgBuf_utf8);
#endif
                (void)SendEndCertPacket(l); //  13:EAPOL Logoff    

                //return eapol();
                return 3;

                break; //should never come here

        }// end switch
    }// end while
}
void Mystar::send_username()
{
    m_state = 1;
    if (isFirstPacketFromServer)	//获得服务器的MAC地址
    {
        memcpy (destMAC, pkt_data + 6, 6);
        isFirstPacketFromServer = 0;
    }
#ifdef DEBUG
    fputs("发送用户包",stdout);
#endif
    (void) SendNamePacket(l, pkt_data);
}
void Mystar::send_password()
{
    m_state = 2;
#ifdef DEBUG
    fputs("发送密码包",stdout);
#endif
    (void)SendPasswordPacket(l, pkt_data);
}
void Mystar::logout(int signo)    //中断信号设置;
{
    char l_errbuf[LIBNET_ERRBUF_SIZE];
    if(m_state==3)
    {
        l=libnet_init(LIBNET_LINK, nic,l_errbuf);
        if( l == NULL)
            _exit(0);
        (void)SendEndCertPacket(l);
        libnet_destroy(l);  //资源释放
    }
    _exit(0);
}
void Mystar::help()
{
    fputs("--nogui  use command line interface\n",stdout);
    fputs("--help   show this help\n",stdout);
}
void Mystar::show_message(const char *message)
{
    printf("%s", message);
}

int Mystar::authen_(void)
{
    if(init_authen()==0)
    {
        int state;
        while(eapol_search());
        while(eapol_sendusername());
        while(state=eapol_sendpassword())
        {
            if(state==3)
                return 1;
        }
    }
    else
        return 1;
}
int Mystar::eapol_search() //TODO:
{
    /*
     * 返回值：
     *  0:成功
     *  1:错误  ,则终止程序
     *  2:超时  ,重连，到一阈值停止,这里应监测网口状态，是否连接？
     *  3:包错误
     * */
    (void)SendFindServerPacket(l);	//the first time to search for server   第一个发送的包包

    (void) sigfillset(&sigset_full);
    (void) sigdelset(&sigset_full, SIGINT);
    FD_ZERO (&read_set);
    FD_SET (p_fd, &read_set);
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;	// 1 second

    //wait with all signals(except SIGINT) blocked.
    switch ( pselect(p_fd+1,&read_set,NULL,NULL,&timeout,&sigset_full) )
    {
        case -1: //Normally, this case should not happen since sig_intr() never returns!
            //error;
            pcap_close(p);
            libnet_destroy(l);
            return 1;
        case 0:  //timed out
            return 2;
    }

    if((pcap_next_ex(p,&pkt_hdr,&pkt_data))!=1) return 3;   //?

    if ((!isFirstPacketFromServer)&&(memcmp(destMAC,pkt_data+6,6)!=0)) return 3;

    if( ( pkt_data[0x12] == 0x01 ) && ( pkt_data[0x16] == 0x01 ) )
    {
        fputs ("@@ Server found, requesting user name...\n", stdout);
        return 0;
    }
    else
        return 3;
}
int Mystar::eapol_sendusername() //TODO:找到EAPoL start，避免死循环   2009-02-23开始
{
    /*
     * 返回值：
     *  0:成功
     *  1:错误  ,则终止程序
     *  2:超时  ,重连，到一阈值停止,这里应监测网口状态，是否连接？
     *  3:包错误
     * */
    send_username();
    (void) sigfillset(&sigset_full);
    (void) sigdelset(&sigset_full, SIGINT);
    FD_ZERO (&read_set);
    FD_SET (p_fd, &read_set);
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;	// 1 second

    //wait with all signals(except SIGINT) blocked.
    switch ( pselect(p_fd+1,&read_set,NULL,NULL,&timeout,&sigset_full) )
    {
        case -1: //Normally, this case should not happen since sig_intr() never returns!
            //error;
            pcap_close(p);
            libnet_destroy(l);
            return 1;
        case 0:  //timed out
            return 2;
    }

    //Here return value of pselect must be 1

    if((pcap_next_ex(p,&pkt_hdr,&pkt_data))!=1) return 3;   //?

    //收到的第二个及其以后的有效packet的源MAC必须等于头次收到的有效分组的源MAC
    if ((!isFirstPacketFromServer)&&(memcmp(destMAC,pkt_data+6,6)!=0)) return 3;

    //received a packet successfully. for convenience, SUPPOSE it's the RIGHT packet!! but maybe WRONG!!
    //for example, we have NEVER vefified the length of packet, fancying the packet's length is 0x11 ?!

    if( ( pkt_data[0x12] == 0x01 ) && ( pkt_data[0x16] == 0x04 ) )
    {
        fputs ("@@ User name valid, requesting password...\n", stdout);
        return 0;
    }
    else
        return 3;
}
int Mystar::eapol_sendpassword() //TODO:找到EAPoL start，避免死循环   2009-02-23开始
{
    /*
     * 返回值：
     *  0:成功
     *  1:错误  ,则终止程序
     *  2:超时  ,重连，到一阈值停止,这里应监测网口状态，是否连接？
     *  3:包错误
     * */
    send_password();
    (void) sigfillset(&sigset_full);
    (void) sigdelset(&sigset_full, SIGINT);
    FD_ZERO (&read_set);
    FD_SET (p_fd, &read_set);
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;	// 1 second

    //wait with all signals(except SIGINT) blocked.
    switch ( pselect(p_fd+1,&read_set,NULL,NULL,&timeout,&sigset_full) )
    {
        case -1: //Normally, this case should not happen since sig_intr() never returns!
            //error;
            pcap_close(p);
            libnet_destroy(l);
            return 1;
        case 0:  //timed out
            return 2;
    }

    //Here return value of pselect must be 1

    if((pcap_next_ex(p,&pkt_hdr,&pkt_data))!=1) return 3;   //?

    //收到的第二个及其以后的有效packet的源MAC必须等于头次收到的有效分组的源MAC
    if ((!isFirstPacketFromServer)&&(memcmp(destMAC,pkt_data+6,6)!=0)) return 3;

    //received a packet successfully. for convenience, SUPPOSE it's the RIGHT packet!! but maybe WRONG!!
    //for example, we have NEVER vefified the length of packet, fancying the packet's length is 0x11 ?!

    if( pkt_data[0x12] == 0x03 )
    {
        fputs("@@ Password valid, authentication SUCCESS!!! :)\n",stdout);
        if (echoInterval<=0)  //user has echo disabled
        {
            //goto done; 
            printf("echoInterval:%d",echoInterval);
            pcap_close(p); libnet_destroy(l);
            return 1;
        }

        //uTemp.ulValue = *(((u_long *)(pkt_data+0x9d)));
        offset=ntohs( *((u_int16_t*)(pkt_data+0x10)) );
        uTemp.ulValue = *((u_int32_t *)(pkt_data+(0x11+offset)-0x08));
        m_key.btValue[0] = Alog(uTemp.btValue[3]);
        m_key.btValue[1] = Alog(uTemp.btValue[2]);
        m_key.btValue[2] = Alog(uTemp.btValue[1]);
        m_key.btValue[3] = Alog(uTemp.btValue[0]);

        //unblock SIGINT, so we can exit with Ctrl+C
        (void)sigemptyset(&sigset_zero);
        (void)sigaddset(&sigset_zero,SIGINT);
        (void)sigprocmask(SIG_UNBLOCK,&sigset_zero,NULL);
        // continue echoing
        fputs("Keeping sending echo... 保持连接了。。。\n",stdout);
        ULONG_BYTEARRAY m_serialNo;
        m_serialNo.ulValue = 0x1000002a;	//the initial serial number, a magic number!
        while(SendEchoPacket(l,pkt_data, &m_serialNo)==0)
            sleep(echoInterval);
        //goto err2; //this should never happen.
        pcap_close(p);
        libnet_destroy(l);
        return 1;
    }
    else if( pkt_data[0x12] == 0x04 )
    {
        msgLen=ntohs( *((u_int16_t*)(pkt_data+0x10)) )-10;
        char *msgBuf_utf8=NULL;
        if (msgLen>0)
        {
            if(msgLen>=(sizeof(msgBuf)-1)) msgLen=sizeof(msgBuf)-1;
            memset(msgBuf,'\0',sizeof(msgBuf));
            memcpy(msgBuf, pkt_data+0x1c, msgLen);
            pmsgBuf=msgBuf;
            //remove the leanding "\r\n" which seems alway to exist!
            if((msgLen>2)&&(msgBuf[0]==0xd)&&(msgBuf[1]==0xa)) pmsgBuf+=2;
        }
        else //这个估计是服务器告知静默或确认断网的包
        {
            pmsgBuf=NULL;
        }
        //fprintf(stdout,"@@ Authenticaton failed!!! :(%s\n",pmsgBuf);
        //fprintf(stdout,"@@ Authenticaton failed!!! :(%s\n",msgBuf_utf8);
        printf("@@ Authenticaton failed!!! :(%s\n",msgBuf_utf8);
        (void)SendEndCertPacket(l); //  13:EAPOL Logoff    
        return 3;
    }
}
int Mystar::get_status()
{
    return m_state;
}
