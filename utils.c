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

    Destroy(ping_info);
    exit(exit_code);
}

void    Init(PingInfo *ping_info)
{
    ping_info->ip_addr = NULL;
    ping_info->socket_fd = 0;
    ping_info->v_opt = 0;
    ping_info->help_opt = 0;
}

void    Destroy(PingInfo *ping_info)
{
    if (ping_info->ip_addr != NULL)
        free(ping_info->ip_addr);
    
    free(ping_info);
}

void    Calc_Checksum(struct icmp *icmp_package, int len)
{
    return
}