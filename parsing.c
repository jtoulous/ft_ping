#include "ft_ping.h"

void    CheckValidOpt(char **argv, PingInfo *ping_info)
{
    int i = 1;
    ping_info->v_opt = 0;
    ping_info->help_opt = 0;

    while (argv[i])
    {    
        if (argv[i][0] == '-')
        {
            int j = 1;
            while (argv[i][j])
            {
                if (argv[i][j] == 'v')
                    ping_info->v_opt = 1;
                
                else if (argv[i][j] == '?')
                    ping_info->help_opt = 1;

                else
                    ExitError("BAD_OPT_ERROR", NULL, argv[i][j], ping_info);
                j++;
            }
        }
        i++;
    }
}

void    CheckHelpOpt(PingInfo *ping_info){
    if (ping_info->help_opt == 1)
    {    
        Destroy(ping_info);
        printf(HELP_MSG);
        exit(0);
    }
}

void    CheckValidHost(char **argv, PingInfo *ping_info)
{
    int                 i = 1;
    struct hostent      *hostend_struct;
    struct sockaddr_in  ipv4_struct;
    struct sockaddr_in  ipv6_struct;

    while (argv[i])
    {
        if (argv[i][0] != '-')
        {
            ping_info->hostname = strdup(argv[i]);
            break ;
        }
    }

    hostend_struct = gethostbyname(ping_info->hostname);
    if (hostend_struct == NULL)
        ping_info->ip_addr = strdup(ping_info->hostname);
    else
        ping_info->ip_addr = inet_ntoa(*(struct in_addr *)hostend_struct->h_addr);

    if (inet_pton(AF_INET, ping_info->ip_addr, &(ipv4_struct.sin_addr)) != 1 
        && inet_pton(AF_INET6, ping_info->ip_addr, &(ipv6_struct.sin_addr)) != 1)
        ExitError("BAD_IP_ERROR", ping_info->ip_addr, '\0', ping_info);
}

void    CheckArguments(int argc, char **argv, PingInfo *ping_info)
{
    if (argc <= 1)
        ExitError("NO_ARGUMENTS_ERROR", NULL, '\0', ping_info);

    CheckValidOpt(argv, ping_info);
    CheckHelpOpt(ping_info);
    CheckValidHost(argv, ping_info);
}