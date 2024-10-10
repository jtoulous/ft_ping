#include "ft_ping.h"


int CheckValidOpt(char **argv, Ping_Info *ping_info)
{
    int i = 0;

    while (argv[i])
    {    
        int j = 0;
        while (argv[i][j])
        {
            if (argv[i][j] != 'v' || argv[i][j] != '?')
            {
                printf("%s%c\n\n%s", BAD_OPT_ERROR, argv[i][j], HELP_MSG)
                exit(0);
            }
        }
    }
}


void    CheckHelpOpt(char **argv){
    int i = 0;

    while (argv[i])
    {
        if (argv[i][0] == "-")
        {
            int j = 0;
            while (argv[i][j])
            {
                if (argv[i][j] == "?")
                {
                    printf(HELP_MSG);
                    exit(0);
                }
            }
            j++;
        }
        i++;
    }
}


void    CheckArguments(int argc,char **argv)
{
    Ping_Info   *ping_info = malloc(sizeof(Ping_Info));
    int         i = 0;

    CheckValidOpt(argv, ping_info);
    CheckHelpOpt(argv, ping_info);
    CheckValidHost(argv, ping_info);
   
    while (argv[i]){
        if (argv[i][0] == '-'){

        }

        else {
            is_ip = inet_pton(AF_INET, )
        }

    }
    
    return;
}