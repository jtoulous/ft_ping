#include "ft_ping.h"

PingInfo ping_info;


void    handle_signals(int sig)
{
    if (sig == SIGINT)
        Signals_State("Activate", "SIGINT");
    else if (sig == SIGQUIT)
        Signals_State("Activate", "SIGQUIT");
}


int     Signals_State(char *action, char *signal)
{
    static int sigint_state = 0;
    static int sigquit_state = 0;
    
    if (strcmp(action, "Init") == 0)
        return (0);

    else if (strcmp(action, "Check") == 0)
    {
        if (strcmp(signal, "SIGINT") == 0)
            return (sigint_state);
        else if (strcmp(signal, "SIGQUIT") == 0)
            return (sigquit_state);
    }

    else if (strcmp(action, "Activate") == 0)
    {
        if (strcmp(signal, "SIGINT") == 0)
            sigint_state++;
        else if (strcmp(signal, "SIGQUIT") == 0)
            sigquit_state++;
    }

    else if (strcmp(action, "Reset") == 0)
    {
        if (strcmp(signal, "SIGINT") == 0)
            sigint_state = 0;
        else if (strcmp(signal, "SIGQUIT") == 0)
            sigquit_state = 0;
    }

    return (0);
}


void    Ping(void)
{
    struct sockaddr_in      dest_addr;
    IcmpPack                icmp_package;
    long double             send_time;
    long double             recv_time;
    
    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&icmp_package, 0, sizeof(icmp_package));

    icmp_package.icmp_type = ICMP_ECHO;
    icmp_package.icmp_code = 0;
    icmp_package.icmp_id = getpid();
    icmp_package.icmp_seq = 0;
    icmp_package.icmp_cksum = 0;

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(ping_info.ip_addr);

    SetupSocket();

    signal(SIGINT, handle_signals);
    signal(SIGQUIT, handle_signals);

    printf("PING %s (%s) %lu(%lu) bytes of data.\n", ping_info.hostname, ping_info.ip_addr, sizeof(icmp_package.data), sizeof(icmp_package) + 20);    
    while (Signals_State("Check", "SIGINT") < 1)
    {
        char                    buffer[100];
        int                     bytes_recv;
        struct sockaddr_in      recv_addr;

        icmp_package.icmp_seq++;
        icmp_package.icmp_cksum = (uint16_t)Calc_Checksum(&icmp_package, sizeof(icmp_package));        

        send_time = Get_Time();
        SendPacket(icmp_package, dest_addr);
        RecvPacket(buffer, sizeof(buffer), &recv_addr, &bytes_recv);
        recv_time = Get_Time();

        PrintRecvInfo(recv_time - send_time, bytes_recv, buffer);
        sleep(1);
    }
    PrintEndingResults();
}


int main(int argc, char **argv)
{
    Init();
    CheckArguments(argc, argv);
    Ping();
    Destroy();
    return 0;
}