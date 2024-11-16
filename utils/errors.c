#include "../ft_ping.h" 

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

    else if (strcmp(type, "TEMP_FAILURE") == 0)
    {
        printf("ping: %s: Temporary failure in name resolution\n", info_str);
        exit_code = 2;
    }

    Destroy();
    exit(exit_code);
}