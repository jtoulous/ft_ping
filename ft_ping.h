#ifndef FT_PING
#define FT_PING

#define BAD_IP_ERROR ""
#define NO_ARGUMENTS_ERROR "ping: usage error: Destination address required\n"
#define HELP_MSG "Options:\n  <destination>      dns name or ip address\n  -v                 verbose output\n"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

typedef struct  s_struct 
{
    char    *ip_addr;
    int     socket_fd;
    int     v_opt;
    int     help_opt;
}   PingInfo;

void    CheckArguments(int argc, char **argv, PingInfo *ping_info);
void    ExitError(char *type, char *info_str, char info_char, PingInfo *ping_info);
void    Init(PingInfo *ping_info);
void    Destroy(PingInfo *ping_info);
void    Calc_Checksum(struct icmp *icmp_package, int len);


#endif