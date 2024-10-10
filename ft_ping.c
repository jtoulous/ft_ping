#include "ft_ping.h"


void    Ping(PingInfo ping_info)
{
    int                     socket_fd;
    struct  sockaddr_in     dest_addr;
    struct  icmp            icmp_package;

    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&icmp_package, 0, sizeof(icmp_package));

    icmp_package.icmp_type = ICMP_ECHO;
    icmp_package.icmp_code = 0;
    icmp_package.icmp_id = getpid();
    icmp_package.icmp_seq = 1;
    icmp_package.icmp_cksum = Calc_Checksum(&icmp_package, sizeof(icmp_package));

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(ping_info.ip_addr);

    ping_info.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)
    if (ping_info.socket_fd < 0)
        ExitError();

    while (1)
    {
        //RECUP TIME 
        if (sendto(ping_info.socket_fd, &icmp_package, sizeof(icmp_package), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) <= 0)
            ExitError();
        //RECUP TIME AND COMPARE AND PRINT
    }
}


int main(int argc, char **argv)
{
    PingInfo   ping_info;

    Init(&ping_info);
    CheckArguments(argc, argv, &ping_info);
//    Ping(ping_info);
    #ICI
    Destroy(ping_info);
    return 0;
}