#include "ft_ping.h"


void    Ping(PingInfo ping_info)
{
    char                    buffer[1000];
    struct sockaddr_in      dest_addr;
    IcmpPack                icmp_package;
    socklen_t               addrlen;

    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&icmp_package, 0, sizeof(icmp_package));
    addrlen = sizeof(dest_addr);

    icmp_package.icmp_type = ICMP_ECHO;
    icmp_package.icmp_code = 0;
    icmp_package.icmp_id = getpid();
    icmp_package.icmp_seq = 1;
    icmp_package.icmp_cksum = 0;
    icmp_package.icmp_cksum = (uint16_t)Calc_Checksum(&icmp_package, sizeof(icmp_package));

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(ping_info.ip_addr);

    ping_info.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_info.socket_fd < 0)
        exit(69);
//        ExitError();

    printf("PING %s (%s) %d(%d) bytes of data.\n", ping_info.hostname, ping_info.ip_addr, sizeof(icmp_package.data), sizeof(icmp_package));
    while (1)
    {
//        long    start = Get_Time();

        if (sendto(ping_info.socket_fd, &icmp_package, sizeof(icmp_package), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) <= 0)
        {
            printf("ERROR SENDTO\n");
            exit(69);
//            ExitError();
        }

        int bytes_recv = recvfrom(ping_info.socket_fd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&dest_addr, &addrlen);
        if(bytes_recv <= 0)
        {
            ExitError();
        }
        long    end = Get_Time();

//        PrintRecvInfo(start, end, bytes_recv, icmp_package, dest_addr)
        sleep(1);
        icmp_package.icmp_seq++;
    }
}


int main(int argc, char **argv)
{
    PingInfo   ping_info;

    Init(&ping_info);
    CheckArguments(argc, argv, &ping_info);
    Ping(ping_info);
    Destroy(&ping_info);
    return 0;
}