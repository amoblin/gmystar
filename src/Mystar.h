#include <stdlib.h>
#include <iostream>

//#include <stdio.h>
//#include <unistd.h>
//#include <string.h>
//#include <signal.h>
//#include <errno.h>
//#include <setjmp.h>
//#include <time.h>
//
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

//#include <arpa/inet.h>

#include <pcap.h>
#include <libnet.h>

#include "User.h"
#include "MD5.h"

#include <dbus/dbus-glib.h>
#include <libnm_glib.h>

#define DEBUG 1

#define FILTER_STR "ether[12:2]=0x888e and ether dst %02x:%02x:%02x:%02x:%02x:%02x"
#define BUFFER_SIZE 4096

using namespace std;

typedef union
{
	u_int32_t   ulValue;
	u_int8_t    btValue[4];
}ULONG_BYTEARRAY;

class Mystar
{
    public:
        Mystar();
        void authen();
        static void help();
        char *gbk2utf8(char *src);
        User user;
        bool autologin;
        int save_file();
        int get_status();
        void logout(int signo);
        libnm_glib_state nm_state;
        libnm_glib_ctx *nm_ctx;

    protected:
        void checkAndSetConfig();//finished!
        void init_file();
        int init_authen();
        int eapol();
        void send_username();
        void send_password();

        int authen_();
        int eapol_sendusername();
        int eapol_sendpassword();
        int eapol_search();

        void show_message(const char *message);

        //sendpacket.c
        int SendFindServerPacket(libnet_t *l);
        int SendNamePacket(libnet_t *l, const u_char *pkt_data);
        int SendPasswordPacket(libnet_t *l,const u_char *pkt_data);
        int SendEchoPacket(libnet_t *l,const u_char *pkt_data, ULONG_BYTEARRAY *m_serialNo);
        int SendEndCertPacket(libnet_t *l);


        //myerr.c
        void err_ret(const char *fmt, ...);
        void err_sys(const char *fmt, ...);
        void err_dump(const char *fmt, ...);
        void err_msg(const char *fmt, ...);
        void err_quit(const char *fmt, ...);
        void err_doit(int, const char*, va_list);


        //blog.h
        void InitializeBlog(const unsigned char *m_ip, const unsigned char *m_netmask,
                const unsigned char *m_netgate, const unsigned char *m_dns1);

        void FillNetParamater(unsigned char ForFill[]);

        unsigned char Alog(unsigned char BForAlog);
        void Blog();

        /*field
         *
         */

        MD5 md5;

        char nic[32];
        int intelligentReconnect;
        int echoInterval;
        int authenticationMode; //是哪种认证模式：0:标准 1:实达

        unsigned char localMAC[6];
        unsigned char destMAC[6];
        unsigned char m_ip[4];
        unsigned char m_netmask[4];
        unsigned char m_netgate[4];
        unsigned char m_dns1[4];

        //Mystar.cc
        libnet_t *l;    //指针，
        pcap_t *p;
        int p_fd;
        u_int32_t l_ip;
        u_int32_t p_netaddr, p_netmask;
        char filter_buf[256];
        struct bpf_program filter_code;

        struct libnet_ether_addr *l_ether_addr;

        char p_errbuf[PCAP_ERRBUF_SIZE];
        char l_errbuf[LIBNET_ERRBUF_SIZE];

        int isFirstPacketFromServer;
        //int packetCount_SentFindServer;
        //int packetCount_SentName;
        //int packetCount_SentPassword;
        //ULONG_BYTEARRAY m_serialNo;
        ULONG_BYTEARRAY m_key;

        volatile sig_atomic_t m_state;//0:disconect 1:found server 2:sending password 3:conect
        sigset_t sigset_full,sigset_zero;


        fd_set read_set;
        struct timespec timeout;
        const u_char *pkt_data;
        struct pcap_pkthdr *pkt_hdr;
        u_int16_t offset;
        ULONG_BYTEARRAY uTemp;
        int msgLen;
        char msgBuf[256];
        char *pmsgBuf;

        char *filename;
};
