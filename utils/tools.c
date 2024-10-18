#include "../ft_ping.h"

void    Init(void)
{
    ping_info.hostname = NULL;
    ping_info.ip_addr = NULL;
    ping_info.socket_fd = -1;
    ping_info.v_opt = 0;
    ping_info.help_opt = 0;
    ping_info.start_time = Get_Time();
    ping_info.ttl_opt = -1;
    ping_info.packets_sent = 0;
    ping_info.packets_received = 0;
    ping_info.min_time = 0;
    ping_info.max_time = 0;
    ping_info.avg_time = 0;
    ping_info.squares_sum = 0;
    ping_info.mdev = 0;
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

void    RecvPacket(char *buffer, int buffer_size, struct sockaddr_in *dest_addr, int *bytes_recv)
{
    socklen_t   addrlen;

    addrlen = sizeof(*dest_addr);
    *bytes_recv = recvfrom(ping_info.socket_fd, buffer, buffer_size, 0, (struct sockaddr *)dest_addr, &addrlen);
    if(*bytes_recv <= 0)
        ExitError("RECV_ERROR", NULL, '\0');
}

void    UpdatePingInfo(double ping_time)
{
    ping_info.packets_received++;

    if (ping_time < ping_info.min_time)
        ping_info.min_time = ping_time;
    if (ping_time > ping_info.max_time)
        ping_info.max_time = ping_time;

    if(ping_info.avg_time == 0)
    {    
        ping_info.avg_time = ping_time;
        ping_info.min_time = ping_time;
        ping_info.max_time = ping_time;
        ping_info.ewma = ping_time;
    }
    else
    {
        ping_info.avg_time = ping_info.avg_time + ((ping_time - ping_info.avg_time) / ping_info.packets_sent);
        ping_info.squares_sum = pow(ping_time - ping_info.avg_time, 2);
        ping_info.mdev = sqrt(ping_info.squares_sum / ping_info.packets_sent);
        ping_info.ewma = 0.1 * ping_time + (1 - 0.1) * ping_info.ewma;
    }
}

char    *DNS_lookup(char *hostname)
{
    char            ip[INET_ADDRSTRLEN];
    unsigned char   response[NS_PACKETSZ];
    int             response_len;
    ns_msg          handle;
    int             answer_count;

    response_len = res_query(hostname, C_IN, T_A, response, sizeof(response));
    if (response_len < 0)
        return (NULL);

    if (ns_initparse(response, response_len, &handle) < 0)
        return (NULL);
    
    answer_count = ns_msg_count(handle, ns_s_an);

    for (int i = 0; i < answer_count; i++)
    {
        ns_rr  rr;
        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0)
            return (NULL);

        if (ns_rr_type(rr) == T_A)
        {
            inet_ntop(AF_INET, ns_rr_rdata(rr), ip, sizeof(ip));
            return strdup(ip);
        }
    }
    return (NULL);
}