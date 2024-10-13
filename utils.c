#include "ft_ping.h"

void    ExitError(char *type, char *info_str, char info_char, PingInfo *ping_info)
{
    int exit_code = 69;

    if (strcmp(type, "BAD_OPT_ERROR") == 0)
    {
        printf("ping: invalid option -- %c\n\n%s\n", info_char, HELP_MSG);
        exit_code = 2;
    }

    else if (strcmp(type, "BAD_IP_ERROR") == 0)
    {
        printf("ping: %s: Name or service not known\n", info_str);
        exit_code = 2;
    }

    else if (strcmp(type, "NO_ARGUMENTS_ERROR") == 0)
    {
        printf("ping: usage error: Destination address required\n");
        exit_code = 1;
    }

    else if (strcmp(type, "SOCKET_ERROR") == 0)
    {
        printf("ping: socket: Operation not permitted\n");
        exit_code = 2;
    }

    else if (strcmp(type, "SENDTO_ERROR") == 0)
    {
        perror("ping: sendto failed\n");
        exit_code = 1;
    }

    else if (strcmp(type, "RECV_ERROR") == 0)
    {
        perror("ping: recvfrom failed\n");
        exit_code = 1;
    }

    Destroy(ping_info);
    exit(exit_code);
}

void    Init(PingInfo *ping_info)
{
    ping_info->hostname = NULL;
    ping_info->ip_addr = NULL;
    ping_info->socket_fd = -1;
    ping_info->v_opt = 0;
    ping_info->help_opt = 0;
    ping_info->total_runtime = 0;
}

void    Destroy(PingInfo *ping_info)
{
    if (ping_info->hostname != NULL)
        free(ping_info->hostname);
    if (ping_info->ip_addr != NULL)
        free(ping_info->ip_addr);
    if (ping_info->socket_fd >= 0)
        close(ping_info->socket_fd);
}

unsigned short    Calc_Checksum(IcmpPack *icmp_package, int len)
{
    unsigned short  *buff;
    unsigned short  res;
    unsigned int    sum;

    icmp_package->icmp_cksum = 0;
    buff = (void *)icmp_package;
    sum = 0;

    while (len > 1)
    {
        sum += *buff;
        buff++;
        len -= 2;
    }
    if (len == 1)
        sum += *(unsigned char *)buff;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    res = ~sum;

    return (res);
}

long double    Get_Time(void)
{
    struct timeval  tv;
    long double          res;

    gettimeofday(&tv, NULL);
    res = (tv.tv_sec * 1000.0f) + (tv.tv_usec / 1000.0f);
    return (res);
}


void    PrintRecvInfo(long double ping_time, int bytes_recv, char *buffer)
{
    struct iphdr    *ip_header;
    IcmpPack        *icmp_package;
    struct hostent  *host;

    ip_header = (struct iphdr *)buffer;
    buffer += 20;
    icmp_package = (IcmpPack *)buffer;

//    ip_addr = inet_ntoa(*(struct in_addr *)&ip_header->saddr);
    host = gethostbyaddr((const void *)&ip_header->saddr, sizeof(ip_header->saddr), AF_INET);

    printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1Lf ms\n",
            bytes_recv - 20,
            host->h_name,
            inet_ntoa(*(struct in_addr *)&ip_header->saddr),
            icmp_package->icmp_seq,
            ip_header->ttl,
            ping_time
        );
}