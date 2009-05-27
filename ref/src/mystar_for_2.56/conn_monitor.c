/*
View this file in UTF-8 !!!
View this file in UTF-8 !!!
View this file in UTF-8 !!!
*/

/*
   
                                        夕霞孤雁
                                       2005.10.20
*/

#include "conn_monitor.h"


static char sendpacket[PACKET_SIZE];
static char recvpacket[PACKET_SIZE];
static int sockfd,datalen=56;
static int nsend=0,nreceived=0;
static struct sockaddr_in dest_addr;
static pid_t pid;
static struct sockaddr_in from;

static void rev_timeout(int signo);
static unsigned short cal_chksum(unsigned short *addr,int len);
static int pack(int pack_no);

static int unpack(char *buf,int len);
static void * ping_thread(void * arg);
static void SetConnectionState(int x);
static char * itoa(n, base);


static int is_still_connected = 1;   /*标识当前是否联接*/
sem_t Mutex, DbgInfo_Mutex;     /*互斥*/
static pthread_t Pinger;
static unsigned int ping_interval = 2;
static int stop_thread_signal = 0;
static unsigned int show_dbg_info_level = 1;
static unsigned int timeout_count = 0;

//Set the interval of ping
void SetInterval(unsigned int Interval)
{
    ping_interval = Interval ;
}


int StartConnectionMonitor()
{
    SetConnectionState(1);
    stop_thread_signal = 0;
    DbgInfo(20,"Start Connection Monitor!");
}

int StopConnectionMonitor()
{
    stop_thread_signal = 1;
}

void * ping_thread(void * arg)
{
    char st[255];
Loop1:

    sprintf(st, "\n ping_thread loop begin!:%d\n", pthread_self());
    DbgInfo(40, st );
    if (stop_thread_signal == 1)
    {
        // stop_thread_signal = 0;
        DbgInfo(1,"Connection_Monitor_thread End!\n");
    }
    else{
        send_packet();
        SetConnectionState( recv_packet() );
        DbgInfo(40, "ping_thread loop end!\n");
    }
    MySleep(ping_interval); //间隔
    goto Loop1;
}

/* 检测网络联接状态，其间信号量互斥  */
int IsStillConnected()
{
    int t;
    sem_wait(&Mutex);
    t = is_still_connected;
    sem_post(&Mutex);
    return t;
}


/* 设置网络联接状态，其间信号量互斥  */
void SetConnectionState(int x)
{
    sem_wait(&Mutex);
    is_still_connected = x;
    sem_post(&Mutex);
}

/* 联接超时 */
void rev_timeout(int signo)
{
    printf("Timeout!    (Count:%d)\n", ++timeout_count);
    SetConnectionState(0);

    pthread_exit(NULL);

    // ping_thread(NULL);  //继续线程
}


/*校验和算法*/
unsigned short cal_chksum(unsigned short *addr,int len)
{       int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;

    /*把ICMP报头二进制数据以2字节为单位累加起来*/
    while(nleft>1)
    {       sum+=*w++;
        nleft-=2;
    }
    /*若ICMP报头为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加*/
    if( nleft==1)
    {       *(unsigned char *)(&answer)=*(unsigned char *)w;
        sum+=answer;
    }
    sum=(sum>>16)+(sum&0xffff);
    sum+=(sum>>16);
    answer=~sum;
    return answer;
}

/*设置ICMP报头*/
int pack(int pack_no)
{       int i,packsize;
    struct icmp *icmp;
    char * s;
    char st[255];

    icmp=(struct icmp*)sendpacket;
    icmp->icmp_type=ICMP_ECHO;
    icmp->icmp_code=0;
    icmp->icmp_cksum=0;
    icmp->icmp_seq=pack_no;
    icmp->icmp_id=pid;

    packsize=8+datalen;
    s = (char *)icmp->icmp_data;
    bzero(s, datalen);
    sprintf(st, "MyStar Conntection Test Packet NO:%d\n", nsend);
    strcpy(s, st);

    DbgInfo(30, "Send Packet:");DbgInfo(30, st);DbgInfo(30, "\n");

    icmp->icmp_cksum=cal_chksum( (unsigned short *)icmp,packsize); /*校验算法*/
    return packsize;
}

/*发送ICMP报文*/
void send_packet()
{       int packetsize;
    nsend++;
    DbgInfo(40,"Send packet\n");
    packetsize=pack(nsend); /*设置ICMP报头*/
    if( sendto(sockfd,sendpacket,packetsize,0,
               (struct sockaddr *)&dest_addr,sizeof(dest_addr) )<0  )
    {
        DbgInfo(1,"sendto error");
        return;
    }
    DbgInfo(20,"Send packet OK!\n");
}

/*接收ICMP报文*/
int recv_packet()
{       int n,fromlen;
    extern int errno;

    if (signal(SIGALRM, rev_timeout) == SIG_ERR)
        DbgInfo(1,"signal Error!\n");
    alarm(MAX_WAIT_TIME);

    fromlen=sizeof(from);
RevAgain:
    DbgInfo(40,"Recive packet\n");
    if( (n=recvfrom(sockfd,recvpacket,sizeof(recvpacket),0,
                    (struct sockaddr *)&from,&fromlen)) <0)
    {
        DbgInfo(1,"recvfrom ErrNO");DbgInfo(1,itoa(errno));
        if(errno==EINTR) return 0;

        goto RevAgain;
    }
    DbgInfo(40,"Unpack\n");
    if(unpack(recvpacket,n) == 0)
    {
        DbgInfo(30,"Unpack faild!\n");
        goto RevAgain;
    }
    else
    {
        DbgInfo(20,"Recive packet OK!\n");
        alarm(0);
        return 1;
    }
}

/*剥去ICMP报头*/
int unpack(char *buf,int len)
{       int i,iphdrlen;
    struct ip *ip;
    struct icmp *icmp;
    char * s;
    char st[255];

    ip=(struct ip *)buf;
    iphdrlen=ip->ip_hl<<2;    /*求ip报头长度,即ip报头的长度标志乘4*/
    icmp=(struct icmp *)(buf+iphdrlen);  /*越过ip报头,指向ICMP报头*/
    len-=iphdrlen;            /*ICMP报头及ICMP数据报的总长度*/
    if( len<8)                /*小于ICMP报头长度则不合理*/
    {       DbgInfo(1,"ICMP packets\'s length is less than 8\n");
        return -1;
    }
    /*确保所接收的是我所发的的ICMP的回应*/
    sprintf(st, "MyStar Conntection Test Packet NO:%d\n", nsend);
    s = (char * )icmp->icmp_data;
    if( (icmp->icmp_type==ICMP_ECHOREPLY) && (icmp->icmp_id==pid)
            && strcmp(s, st)==0 )
    {
        DbgInfo(40,"Get Packet:");DbgInfo(40,st);DbgInfo(40,"\n");
        return 1;
    }
    else
    {
        if (icmp->icmp_id == pid)
            DbgInfo(20,"Not My Pack!\n");
        return 0;
    }
}


int OpenAlarmSignal()
{
    sigset_t   sigset_alarm;
    sigemptyset(&sigset_alarm);
    if (sigaddset(&sigset_alarm,SIGALRM) == -1)
        return -1;
    if (sigprocmask(SIG_UNBLOCK, &sigset_alarm, NULL) != 0)
        return -1;
    else
        return 0;
}

void init_dbg(unsigned int dbg_level)
{
    show_dbg_info_level = dbg_level;
    sem_init(&DbgInfo_Mutex, 0, 1);
}


int ConnectionMonitor_init(char * desthost, unsigned int dbg_level)
{
    struct hostent *host;
    struct protoent *protocol;
    int waittime=MAX_WAIT_TIME;
    int size=50*1024;
    unsigned long int inaddr=0l;
    init_dbg(dbg_level);

    DbgInfo(1,"ConnectionMonitor_init\n");

    if( (protocol=getprotobyname("icmp") )==NULL)
    {
        DbgInfo(1,"getprotobyname");
        return 0;
    }

    /*生成使用ICMP的原始套接字,这种套接字只有root才能生成*/
    if( (sockfd=socket(AF_INET,SOCK_RAW,protocol->p_proto) )<0)
    {       DbgInfo(1,"socket error");
        return 0;
    }

    /* 回收root权限,设置当前用户权限*/
    setuid(getuid());
    /*扩大套接字接收缓冲区到50K这样做主要为了减小接收缓冲区溢出的
      的可能性,若无意中ping一个广播地址或多播地址,将会引来大量应答*/
    setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size) );
    bzero(&dest_addr,sizeof(dest_addr));
    dest_addr.sin_family=AF_INET;

    /*判断是主机名还是ip地址*/
    DbgInfo(10, "Dest Host:");DbgInfo(10, desthost);DbgInfo(10, "\n");
    if( (inaddr=inet_addr(desthost)) == INADDR_NONE)
        {       if((host=gethostbyname(desthost) ) == NULL) /*是主机名*/
        {       DbgInfo(1, "gethostbyname error\n");
            return 0;
        }
        memcpy( (char *)&dest_addr.sin_addr,host->h_addr,host->h_length);
    }
    else    /*是ip地址*/
    {
        memcpy( (char *)&dest_addr.sin_addr, (char *)&inaddr, sizeof(inaddr));
    }
    DbgInfo(10, "Dest Host IP:");DbgInfo(10, inet_ntoa(dest_addr.sin_addr));DbgInfo(10, "\n");

    /*获取main的进程id,用于设置ICMP的标志符*/
    pid=getpid();
    OpenAlarmSignal();


    sem_init(&Mutex, 0, 1);
    stop_thread_signal = 1;
    pthread_create(&Pinger, NULL, ping_thread, NULL);

    return 1;
}

void MySleep(unsigned int Interval)
{
    struct timespec t;
    t.tv_sec = Interval; //暂停Interval秒
    t.tv_nsec = 0; //0纳秒
    nanosleep(&t, NULL);
}


void DbgInfo(int level, const char * s)
{
    sem_wait(&DbgInfo_Mutex);
    if ((show_dbg_info_level > 0) && (level <= show_dbg_info_level))
    {
        fputs(s,stdout);
    }
    sem_post(&DbgInfo_Mutex);
}

/*void DbgInfo(int level, char * fmt, ...)
{
   sem_wait(&DbgInfo_Mutex);
   if ((show_dbg_info_level > 0) && (level <= show_dbg_info_level))
    {
       va_list argp;
       va_start(argp, fmt);
       printf(fmt, argp);
       va_end(argp);
    }
   sem_post(&DbgInfo_Mutex);
}*/

char * itoa(n, base)
long n;   /* abs k16 */
int base;
{
    register char *p;
    register int minus;
    static char buf[36];

    p = &buf[36];
    *--p = '\0';
    if (n < 0)
    {
        minus = 1;
        n = -n;
    }
    else
        minus = 0;
    if (n == 0)
        *--p = '0';
    else
        while (n > 0)
        {
            *--p = "0123456789abcdef"[n % base];
            n /= base;
        }
    if (minus)
        *--p = '-';
    return p;
}
