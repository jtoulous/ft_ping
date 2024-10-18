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
//    struct hostent      *hostend_struct;
    struct sockaddr_in  ipv4_struct;
    struct sockaddr_in  ipv6_struct;

    while (argv[i])
    {
        if (argv[i][0] != '-')
        {
            ping_info.hostname = strdup(argv[i]);
            break ;
        }
    }

//    hostend_struct = gethostbyname(ping_info.hostname);
    ping_info.ip_addr = DNS_lookup(ping_info.hostname);
    if (ping_info.ip_addr == NULL)
        ping_info.ip_addr = strdup(ping_info.hostname);
//    else
//        ping_info.ip_addr = strdup(inet_ntoa(*(struct in_addr *)hostend_struct->h_addr));

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
    CheckValidHost(argv);
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