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
#include <fcntl.h>
#include <math.h>

#include <resolv.h>
#include <netinet/in.h>
#include <arpa/nameser.h>

typedef struct  p_struct 
{
    char    *hostname;
    char    *ip_addr;
    
    int     socket4_fd;
    char    *socket4_type;
    char    *socket4_family;
    
    int     socket6_fd;
    char    *socket6_type;
    char    *socket6_family;

    int     packets_sent;
    int     packets_received;
    double    start_time;
    double    min_time;
    double    max_time;
    double    avg_time;
    double    squares_sum;
    double    mdev;
    double    ewma;

    int     v_opt;
    int     help_opt;
    int     ttl_opt;
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
double              Get_Time(void);
int                 Signals_State(char *action, char *signal);
void                handle_signals(int sig);
void                SetupSocket(void);
void                SendPacket(IcmpPack icmp_package, struct sockaddr_in dest_addr);
void                RecvPacket(char *buffer, int buffer_size, struct sockaddr_in *dest_addr, int *bytes_recv);
void                UpdatePingInfo(double ping_time);
char                *DNS_lookup(char *hostname);

void                PrintStart(IcmpPack icmp_package);
void                PrintEndingResults(void);
void                PrintRecvInfo(double ping_time, int bytes_recv, char *buffer);
void                PrintVopt_part1(void);
void                PrintVopt_part2(void);
int                 IsIp(char *str);

#endif