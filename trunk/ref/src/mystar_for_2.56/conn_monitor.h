/*
View this file in UTF-8 !!!
View this file in UTF-8 !!!
View this file in UTF-8 !!!
*/

/*
  首先感谢SnowWings、netxray等前辈做出的努力。我一直在用MyStar。但是不只道是不是我这边的网络太差劲还是别的原因，总之经
常掉线。MyStar中提到了智能重连功能，但是没有实现。这个功能对我很重要，最近抽时间给MyStar加入了自动重连的功能。
 
在此说明几点：
   1、自动重连的原理：每间隔几秒钟测试能否ping通某网络结点，如果不能够ping通则说明网络断开，随后从新进行网络认证。
   2、此网络结点必须在mystar.conf文件中设置，如：“intelligentHost=218.197.105.1”， 结点的名称可以是IP地址也可以是域名，一般设置为本机所处网络的网关。
   3、ping网络结点的代码单独运行在一个线程中。
   4、由于实用了多线程，所以修改了Makfile文件，加入POSIX线程库 -lpthread
   5、由于是多线程环境，所以不能再使用非线程安全的sleep函数进行延时，而使用了自定义函数MySleep，此函数最终通过nanosleep进行系统调用，可以保证线程安全。
   6、mystar.c文件中的一处sleep也被替换为MySleep。
   7、mystar.c关闭了所有的signal，但由于要使用SIGALRM信号，所有在ConnectionMonitor_init函数中又打开了SIGALRM信号。
   8、sleep函数干扰SIGALRM信号，这也是不能再使用sleep的原因之一。
   9、为方便调试，专门编写了输出调试信息的函数，同时需要在mystar.conf文件中设置输出调试信息的级别，1表示最高，0为不输出。默认不输出。
   10、mystar.c 添加了两个变量static char  *m_intelligentHost=NULL; 和static unsigned int m_DbgInfoLevel = 0;分别标识“被测试的网络结点地址”和“调试信息的级别”。checkAndSetConfig()函数也进行了相应的调整。
   11、流程
              认证成功
                 ↓
              初始化，并传入相关参数：被监测结点、输出调试信息级别ConnectionMonitor_init(m_intelligentHost, m_DbgInfoLevel);
                 ↓
              设置探测网络结点的间隔时间        SetInterval(2);
                 ↓
              启动线程  StartConnectionMonitor();
                 ↓
              while(SendEchoPacket(l,pkt_data)==0)
              {
                  延时
                   ↓
                  判定是否能够联接被监测网络结点 IsStillConnected()
                  如果 否：
                         {
                              停止监测   StopConnectionMonitor();
                              从新认证   goto beginAuthentication;
                         }
              }   
 
   12、本人并不熟悉Linux环境下的编程，纰漏之处还望方家多多指教。
                                                      夕霞孤雁
                                                      2005.10.20
*/

#ifndef CONN_MONITOR_H
#define CONN_MONITOR_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#define PACKET_SIZE     4096
#define MAX_WAIT_TIME   5
#define MAX_NO_PACKETS  3

int  IsStillConnected(void);  /* 判定当前是否联接网络 */
int  StartConnectionMonitor(void); /* 开始监测 */
int  StopConnectionMonitor(void);  /* 停止监测 */
int  ConnectionMonitor_init(char * desthost, unsigned int dbg_level);/*初始化，desthost传入网关或其他可以监测网络联接状态的地址（域名或IP）；dbg_level标识输出调试信息的级别，0为禁止输出调试信息*/
void SetInterval(unsigned int Interval);/*设置间隔时间*/
void MySleep(unsigned int Interval);/*替换非线程安全的sleep*/
void DbgInfo(int level, const char * s);/*用于输出调试信息，levle指使调试信息的级别，1为最高;此函数线程安全*/
//void DbgInfo(int level, char * fmt, ...);/*用于输出调试信息，levle指使调试信息的级别，1为最高，后面的参数与printf相同。此函数线程安全*/


static void send_packet(void);
static int recv_packet(void);

#endif /* CONN_MONITOR_H */
