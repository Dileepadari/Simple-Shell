#include "headers.h"
char home_dir[100];
char *gethome_len()
{
    readlink("/proc/self/exe", home_dir, sizeof(home_dir));
    char *abs_home = strrchr(home_dir, '/');
    *abs_home = '\0';
    return home_dir;
}

void prompt(int *prev_prom, char *prev_prom_msg)
{
    char sys_name[255];
    char present_dir[500];
    char *user = getlogin();
    gethostname(sys_name, sizeof(sys_name));
    getcwd(present_dir, sizeof(present_dir));
    if (*prev_prom == 1)
    {
        printf("<%s@%s:~%s %s >", user, sys_name, present_dir + strlen(gethome_len()), prev_prom_msg);
        *prev_prom = 0;
        return;
    }
    if (strlen(present_dir) >= strlen(home_dir))
    {
        printf("<%s@%s:~%s>", user, sys_name, present_dir + strlen(gethome_len()));
    }
    else
    {
        printf("<%s@%s:%s>", user, sys_name, present_dir);
    }
}