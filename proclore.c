#include "headers.h"

void proclore(char **command, int bg, int cmd_count, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    int pid = 0;
    if (cmd_count == 1)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(command[1]);
    }

    char virtual_path[256];
    snprintf(virtual_path, sizeof(virtual_path), "/proc/%d/statm", pid);

    FILE *vir_file = fopen(virtual_path, "r");

    unsigned long virtual_size;
    if (vir_file)
    {
        fscanf(vir_file, "%lu", &virtual_size);
        fclose(vir_file);
    }

    char status_path[256];
    char status[8];
    snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);
    FILE *status_file = fopen(status_path, "r");
    if (status_file)
    {
        char line[280];
        while (fgets(line, sizeof(line), status_file))
        {
            if (strncmp(line, "State:", 6) == 0)
            {
                sscanf(line + 7, "%s", status);
                break;
            }
        }
        fclose(status_file);
    }

    char proc_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);
    readlink(proc_path, proc_path, sizeof(proc_path) - 1);
    char *home_dir = gethome_len();
    if (strlen(proc_path) >= strlen(home_dir))
    {
        snprintf(proc_path, sizeof(proc_path), "~%s", proc_path + strlen(home_dir));
    }
    pid_t group_pid = tcgetpgrp(STDIN_FILENO);
    ;
    char plus = '\0';
    if (pid == group_pid)
    {
        plus = '+';
    }
    else
    {
        plus = '\0';
    }

    printf("pid : %d\n", pid);
    printf("Process status : %s%c\n", status, plus);
    printf("Process Group : %d\n", getpgid(pid));
    printf("Virtual memory : %ld\n", virtual_size);
    printf("executable path : %s\n", proc_path);
}
