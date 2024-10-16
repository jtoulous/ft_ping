#include "ft_ping.h"

void    ExitError(char *type, char *info_str, char info_char)
{
    int exit_code = 69;

    if (strcmp(type, "BAD_OPT_ERROR") == 0)
    {
        printf("ping: invalid option -- %c\n\n%s\n", info_char, HELP_MSG);
        exit_code = 2;
    }

    else if (strcmp(type, "BAD_ARG_ERROR") == 0)
    {
        printf("ping: invalid argument: '%s'\n", info_str);
        exit_code = 1;
    }

    else if (strcmp(type, "NO_ARG_OPT") == 0)
    {
        printf("ping: option requires an argument -- '%c'\n\n%s\n", info_char, HELP_MSG);
        exit_code = 2;
    }

    else if (strcmp(type, "INVALID_TTL_VAL") == 0)
    {
        printf("ping: invalid argument: '%s': out of range: 0 <= value <= 255\n", info_str);
        exit_code = 1;
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
        printf("ping: sendto failed\n");
        exit_code = 1;
    }

    else if (strcmp(type, "RECV_ERROR") == 0)
    {
        printf("ping: recvfrom failed\n");
        exit_code = 1;
    }

    else if (strcmp(type, "SIGACTION ERROR") == 0)
    {
        printf("ping: sigaction init failed\n");
        exit_code = 1;
    }

    else if (strcmp(type, "SOCKET_OPT_ERROR") == 0)
    {
        printf("ping: setsockopt() failed\n");
        exit_code = 1;
    }

    Destroy();
    exit(exit_code);
}

void    Init(void)
{
    ping_info.hostname = NULL;
    ping_info.ip_addr = NULL;
    ping_info.socket_fd = -1;
    ping_info.v_opt = 0;
    ping_info.help_opt = 0;
    ping_info.total_runtime = 0;
    ping_info.ttl_opt = -1;
}

void    Destroy(void)
{
    if (ping_info.hostname != NULL)
        free(ping_info.hostname);
    if (ping_info.ip_addr != NULL)
        free(ping_info.ip_addr);
    if (ping_info.socket_fd >= 0)
        close(ping_info.socket_fd);
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

double    Get_Time(void)
{
    struct  timeval  tv;
    double        res;

    gettimeofday(&tv, NULL);
    res = (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
    return (res);
}


void    Print_Sigquit(void)
{
    printf("\nNique ta soeur\n");
//        printf("%d/%d packets, %d%% loss", //min/avg/ewma/max = %lu/%lu/%lu/%lu ms\n", 
//            ping_info.packets_sent,
//            ping_info.packets_received,
//            ((ping_info.packets_sent - ping_info.packets_received) / ping_info.packets_sent) * 100);
//            69.69,
//            69.69,
//            69.69,
//            69.69
//        );
    Signals_State("Reset", "SIGQUIT");   
}

void    PrintRecvInfo(double ping_time, int bytes_recv, char *buffer)
{
    struct iphdr    *ip_header;
    IcmpPack        *icmp_package;

    if(Signals_State("Check", "SIGINT") >= 1)
        return;

    if(Signals_State("Check", "SIGQUIT") >= 1)
        Print_Sigquit();

    ip_header = (struct iphdr *)buffer;
    buffer += 20;
    icmp_package = (IcmpPack *)buffer;

    printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1f ms\n",
            bytes_recv - 20,
            ping_info.hostname,
            inet_ntoa(*(struct in_addr *)&ip_header->saddr),
            icmp_package->icmp_seq,
            ip_header->ttl,
            ping_time
        );
}


void SetupSocket(void)
{
    ping_info.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_info.socket_fd < 0)
        ExitError("SOCKET_ERROR", NULL, '\0');

    if (ping_info.ttl_opt != -1)
        if (setsockopt(ping_info.socket_fd, IPPROTO_IP, IP_TTL, &ping_info.ttl_opt, sizeof(ping_info.ttl_opt)) < 0)
            ExitError("SOCKET_OPT_ERROR", NULL, '\0');
}

void    SendPacket(IcmpPack icmp_package, struct sockaddr_in dest_addr)
{
    if (sendto(ping_info.socket_fd, &icmp_package, sizeof(icmp_package), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) <= 0)
        ExitError("SENDTO_ERROR", NULL, '\0');
    ping_info.packets_sent++;
}

void    RecvPacket(char *buffer, struct sockaddr_in *dest_addr, int *bytes_recv)
{
    socklen_t   addrlen;

    addrlen = sizeof(dest_addr);
    *bytes_recv = recvfrom(ping_info.socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)dest_addr, &addrlen);
    if(*bytes_recv <= 0)
        ExitError("RECV_ERROR", NULL, '\0');
    else
        ping_info.packets_received++;
}