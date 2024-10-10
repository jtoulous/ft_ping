#ifndef FT_PING
#define FT_PING


#define BAD_OPT_ERROR "ping: invalid option -- "
#define HELP_MSG "Options:\n  <destination>      dns name or ip address\n  -v                 verbose output"

struct  Ping_Info 
{
    char    *ip_addr;
    int     v_opt;
    int     ?_opt;
};

void    CheckArguments(int argc,char **argv);


#endif