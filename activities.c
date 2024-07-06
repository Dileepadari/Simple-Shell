#include "headers.h"

int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

void get_full_stat(char *state_status, char *state)
{
    if (!strcmp(state, "R"))
    {
        strcpy(state_status, "Running");
    }
    else if (!strcmp(state, "Z"))
    {
        strcpy(state_status, "Stopped");
    }
    else if (!strcmp(state, "S"))
    {
        strcpy(state_status, "Running");
    }
    else
    {
        strcpy(state_status, "Stopped");
    }
}

void activities()
{
    char shell_pid[32];
    int pid = getpid();
    sprintf(shell_pid, "%d", pid);

    struct dirent **namelist;
    int n;

    n = scandir("/proc", &namelist, NULL, NULL);
    if (n < 0)
    {
        perror("scandir");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (is_digit(namelist[i]->d_name[0]))
        {
            char status_path[280];
            sprintf(status_path, "/proc/%s/status", namelist[i]->d_name);

            FILE *file = fopen(status_path, "r");
            if (file)
            {
                char line[256];
                char cmd_name[256] = "";
                char state[32] = "";
                int is_shell_process = 0;

                while (fgets(line, sizeof(line), file) != NULL)
                {
                    if (!strncmp(line, "Name:", 5))
                    {
                        sscanf(line, "Name:\t%s", cmd_name);
                    }
                    else if (!strncmp(line, "State:", 6))
                    {
                        sscanf(line, "State:\t%s", state);
                    }
                    else if (!strncmp(line, "PPid:", 5))
                    {
                        if (!strcmp(shell_pid, strtok(line + 6, " \n")))
                        {
                            is_shell_process = 1;
                        }
                    }
                }

                fclose(file);
                char state_status[20];
                get_full_stat(state_status, state);
                if (is_shell_process)
                {
                    printf("[%s] : %s - %s\n", namelist[i]->d_name, cmd_name, state_status);
                }
            }
            free(namelist[i]);
        }
    }

    free(namelist);
}
