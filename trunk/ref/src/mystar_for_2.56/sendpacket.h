#include <string.h>
#include <sys/types.h>
#include <libnet.h>

/**********************************************************************************************
  Those 5 functions below return 0 if ok, -1 if fail. However they should never fail normally,
  so we usually ignore the return values JUST FOR CONVENIENCE.
  If detecting the errors,which might happen, is very important to your program, don't ingore it.
**********************************************************************************************/

int SendFindServerPacket(libnet_t *l);
int SendNamePacket(libnet_t *l, const u_char *pkt_data);
int SendPasswordPacket(libnet_t *l,const u_char *pkt_data);
int SendEchoPacket(libnet_t *l,const u_char *pkt_data);
int SendEndCertPacket(libnet_t *l);

