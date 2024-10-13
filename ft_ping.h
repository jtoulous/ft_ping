#ifndef FT_PING
#define FT_PING

#define BAD_IP_ERROR ""
#define NO_ARGUMENTS_ERROR "ping: usage error: Destination address required\n"
#define HELP_MSG "Options:\n  <destination>      dns name or ip address\n  -v                 verbose output\n"
#define ICMP_ECHO 8

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netinet/ip.h>

typedef struct  p_struct 
{
    char    *hostname;
    char    *ip_addr;
    int     socket_fd;
    int     v_opt;
    int     help_opt;
    long    total_runtime;
}   PingInfo;

typedef struct i_struct
{
    uint8_t     icmp_type;
    uint8_t     icmp_code;
    uint16_t    icmp_cksum;
    uint16_t    icmp_id;
    uint16_t    icmp_seq;
    char        data[56];
}   IcmpPack;

void                CheckArguments(int argc, char **argv, PingInfo *ping_info);
void                ExitError(char *type, char *info_str, char info_char, PingInfo *ping_info);
void                Init(PingInfo *ping_info);
void                Destroy(PingInfo *ping_info);
unsigned short      Calc_Checksum(IcmpPack *icmp_package, int len);
long double              Get_Time(void);
void                PrintRecvInfo(long double ping_time, int bytes_recv, char *buffer);

#endif