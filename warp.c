#include "headers.h"
char prev_dir[100];
char pres_dir[100];

int warp(char **command, int bg, int cmd_len, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    char home_dir[100];
    readlink("/proc/self/exe", home_dir, sizeof(home_dir));
    char *abs_home = strrchr(home_dir, '/');
    *abs_home = '\0';
    getcwd(pres_dir, sizeof(pres_dir));
    if (cmd_len == 1 || (cmd_len == 2 && !strcmp(command[1], "~")))
    {
        getcwd(prev_dir, sizeof(prev_dir));
        printf("%s\n", home_dir);
        chdir(home_dir);
    }
    else if (cmd_len == 2 && !strcmp(command[1], "."))
    {
        getcwd(prev_dir, sizeof(prev_dir));
        printf("%s\n", pres_dir);
    }
    else if (cmd_len == 2 && !strcmp(command[1], ".."))
    {
        getcwd(prev_dir, sizeof(prev_dir));
        char *temp = strrchr(pres_dir, '/');
        *temp = '\0';
        printf("%s\n", pres_dir);
        chdir(pres_dir);
    }
    else if (cmd_len == 2 && !strcmp(command[1], "-"))
    {
        if (prev_dir == NULL)
        {
            strcpy(prev_dir, home_dir);
        }
        chdir(prev_dir);
        printf("%s\n", prev_dir);
        strcpy(prev_dir, pres_dir);
    }
    else if (cmd_len == 2 && (!chdir(command[1])))
    {
        getcwd(prev_dir, sizeof(prev_dir));
        chdir(command[1]);
        getcwd(pres_dir, sizeof(pres_dir));
        printf("%s\n", pres_dir);
    }
    else if (cmd_len == 2 && (command[1][0] == '~') && (command[1][1] == '/'))
    {
        getcwd(prev_dir, sizeof(prev_dir));
        chdir(home_dir);
        if (chdir(command[1] + 2))
        {
            printf("No such folder\n");
        }
        else
        {
            getcwd(pres_dir, sizeof(pres_dir));
            printf("%s\n", pres_dir);
        }
    }
    else if (cmd_len > 2)
    {
        for (int i = 1; i < cmd_len; i++)
        {
            getcwd(prev_dir, sizeof(prev_dir));
            if (!chdir(command[i]))
            {
                getcwd(pres_dir, sizeof(pres_dir));
                printf("%s\n", pres_dir);
            }
            else if ((command[i][0] == '~') && (command[i][1] == '/'))
            {
                chdir(home_dir);

                if (chdir(command[i] + 2))
                {
                    printf("No such folder\n");
                }
                else
                {
                    getcwd(pres_dir, sizeof(pres_dir));
                    printf("%s\n", pres_dir);
                }
            }
            else
            {
                printf("No such folder\n");
                break;
            }
        }
    }
    else if (cmd_len == 2 && chdir(command[1]))
    {
        printf("No such Folder Present\n");
    }
    return 1;
}
