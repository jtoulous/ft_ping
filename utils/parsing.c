#include "../ft_ping.h"

void    CheckTTLOpt(int *i, int *j, int argc, char **argv)
{
    int     ttl;

    if (argv[*i][*j + 1] != '\0')
    {    
        char *tmp = &argv[*i][*j + 1];
        ExitError("BAD_ARG_ERROR", tmp, '\0');
    }
    if (*i == argc - 1)
        ExitError("NO_ARG_OPT", NULL, 't');

    if (strcmp(argv[*i + 1], "0") == 0)
        ttl = 0;
    else
    {
        ttl = atoi(argv[*i + 1]);
        if (ttl == 0)
            ExitError("BAD_ARG_ERROR", argv[*i + 1], '\0');

        if (ttl > 255 || ttl < 0)
            ExitError("INVALID_TTL_VAL", argv[*i + 1], '\0');

    }
    ping_info.ttl_opt = ttl;
    *i += 1;
}

void    CheckValidOpt(int argc, char **argv)
{
    int i = 1;
    ping_info.v_opt = 0;
    ping_info.help_opt = 0;

    while (argv[i])
    {    
        if (argv[i][0] == '-')
        {
            int j = 1;
            while (argv[i][j])
            {
                if (argv[i][j] == 'v')
                    ping_info.v_opt = 1;
                
                else if (argv[i][j] == '?')
                    ping_info.help_opt = 1;

                else if (argv[i][j] == 't')
                {
                    CheckTTLOpt(&i, &j, argc, argv);
                    break;
                }

                else
                    ExitError("BAD_OPT_ERROR", NULL, argv[i][j]);
                j++;
            }
        }
        i++;
    }
}

void    CheckHelpOpt(void){
    if (ping_info.help_opt == 1)
    {    
        Destroy();
        printf(HELP_MSG);
        exit(0);
    }
}

void    CheckValidHost(char **argv)
{
    int                 i = 1;
    struct sockaddr_in  ipv4_struct;
    struct sockaddr_in  ipv6_struct;

    while (argv[i])
    {
        if (argv[i][0] != '-')
        {
            ping_info.hostname = strdup(argv[i]);
            break ;
        }
        i++;
    }

    if (IsIp(ping_info.hostname) == 0)
    {
        ping_info.ip_addr = DNS_lookup(ping_info.hostname);
        if (ping_info.ip_addr == NULL)
            ExitError("TEMP_FAILURE", ping_info.hostname, '\0');
    }
    else
        ping_info.ip_addr = strdup(ping_info.hostname);

    if (inet_pton(AF_INET, ping_info.ip_addr, &(ipv4_struct.sin_addr)) != 1 
        && inet_pton(AF_INET6, ping_info.ip_addr, &(ipv6_struct.sin_addr)) != 1)
        ExitError("BAD_IP_ERROR", ping_info.ip_addr, '\0');
}

void    CheckArguments(int argc, char **argv)
{
    if (argc <= 1)
        ExitError("NO_ARGUMENTS_ERROR", NULL, '\0');

    CheckValidOpt(argc, argv);
    CheckHelpOpt();
    SetupSocket();
    
    if (ping_info.v_opt == 1)
        PrintVopt_part1();

    CheckValidHost(argv);

    if (ping_info.v_opt == 1)
        PrintVopt_part2();

}