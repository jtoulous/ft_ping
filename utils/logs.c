#include "../ft_ping.h"

void    Print_Sigquit(void)
{
    printf("\n%d/%d packets, %d%% loss", 
        ping_info.packets_received,
        ping_info.packets_sent,
        ((ping_info.packets_sent - ping_info.packets_received) / ping_info.packets_sent) * 100
    );

    if (ping_info.avg_time != 0)
    {
        printf(", min/avg/ewma/max = %.3f/%.3f/%.3f/%.3f ms", 
            ping_info.min_time,
            ping_info.avg_time,
            ping_info.ewma,
            ping_info.max_time
        );
    }
    printf("\n");

    Signals_State("Reset", "SIGQUIT");   
}

void    PrintRecvInfo(double ping_time, int bytes_recv, char *buffer)
{
    struct iphdr    *ip_header;
    IcmpPack        *icmp_package;

    ip_header = (struct iphdr *)buffer;
    buffer += 20;
    icmp_package = (IcmpPack *)buffer;

    if (icmp_package->icmp_type == 0)
        UpdatePingInfo(ping_time);

    if(Signals_State("Check", "SIGINT") >= 1)
        return;

    if(Signals_State("Check", "SIGQUIT") >= 1)
        Print_Sigquit();

    if (icmp_package->icmp_type == 0)
    {
        printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1f ms\n",
                bytes_recv - 20,
                ping_info.hostname,
                inet_ntoa(*(struct in_addr *)&ip_header->saddr),
                icmp_package->icmp_seq,
                ip_header->ttl,
                ping_time
            );
    }

    if  (icmp_package->icmp_type == 11)
    {
        IcmpPack    *failed_package = (IcmpPack *)(buffer + 8 + sizeof(struct iphdr));
        
        printf("From %s (%s) icmp_seq=%d Time to live exceeded\n",
            inet_ntoa(*(struct in_addr *)&ip_header->saddr), 
            inet_ntoa(*(struct in_addr *)&ip_header->saddr), 
            failed_package->icmp_seq
        );
    }
}


void    PrintEndingResults(void)
{
    if (ping_info.packets_sent - ping_info.packets_received == 0)
    {
        printf("\n--- %s ping statistics ---\n%d packets transmitted, %d received, %d%% packet loss, time %dms\nrtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
            ping_info.hostname,
            ping_info.packets_sent,
            ping_info.packets_received,
            ((ping_info.packets_sent - ping_info.packets_received) / ping_info.packets_sent) * 100,
            (int)(Get_Time() - ping_info.start_time),
            ping_info.min_time,
            ping_info.avg_time,
            ping_info.max_time,
            ping_info.mdev
        );
    }

    else
    {
        printf("\n--- %s ping statistics ---\n%d packets transmitted, %d received, +%d errors, %d%% packet loss, time %dms\n",
            ping_info.hostname,
            ping_info.packets_sent,
            ping_info.packets_received,
            ping_info.packets_sent - ping_info.packets_received,
            ((ping_info.packets_sent - ping_info.packets_received) / ping_info.packets_sent) * 100,
            (int)(Get_Time() - ping_info.start_time)
        );
    }
}

void    PrintStart(IcmpPack icmp_package)
{
    printf("PING %s (%s) %lu(%lu) bytes of data.\n", ping_info.hostname, ping_info.ip_addr, sizeof(icmp_package.data), sizeof(icmp_package) + 20);
}

void    PrintVopt(void)
{
    printf("ping: sock4.fd: %d (socktype: %s), sock6.fd: %d (socktype: %s), hints.ai_family: AF_UNSPEC\n\nai->ai_family: %s, ai->ai_canonname: '%s'\n",
            ping_info.socket4_fd,
            ping_info.socket4_type,
            ping_info.socket6_fd,
            ping_info.socket6_type,
            ping_info.socket4_family,
            ping_info.hostname
        );
}