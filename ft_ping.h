#ifndef FT_PING
#define FT_PING

#define BAD_IP_ERROR ""
#define NO_ARGUMENTS_ERROR "ping: usage error: Destination address required\n"
#define HELP_MSG "Options:\n  <destination>      dns name or ip address\n  -v                 verbose output\n"
#define ICMP_ECHO 8

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
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
    long    total_runtime;
    int     packets_sent;
    int     packets_received;
    long    min_time;
    long    max_time;
    long    avg_time;
    long    ewma;

    int     v_opt;
    int     help_opt;
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

extern PingInfo     ping_info;

void                CheckArguments(int argc, char **argv);
void                ExitError(char *type, char *info_str, char info_char);
void                Init(void);
void                Destroy(void);
unsigned short      Calc_Checksum(IcmpPack *icmp_package, int len);
long double         Get_Time(void);
void                PrintRecvInfo(long double ping_time, int bytes_recv, char *buffer);
int                 Signals_State(char *action, char *signal);
void                OpenStdout(void);
void                CloseStdout(void);

#endif