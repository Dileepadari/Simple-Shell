#include "headers.h"

void neonate(char *time_halt)
{
    int fg_present;
    fg_present = 0;
    char *hi;
    int pid = fork();
    if (pid == 0)
    {
        rawmode();
    }
    else
    {
        while (1)
        {

            unsigned int recent_pid;
            FILE *file = fopen("/proc/loadavg", "r");
            if (file == NULL)
            {
                perror("fopen");
                return;
            }

            char buffer[128];
            if (fgets(buffer, sizeof(buffer), file) != NULL)
            {
                sscanf(buffer, "%*f %*f %*f %*d%*c%*d %u", &recent_pid);
            }
            else
            {
                printf("Failed to read /proc/loadavg\n");
            }

            fclose(file);
            int normality;
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            pid_t completed_pid = waitpid(pid, &normality, WNOHANG);
            if (completed_pid > 0)
            {
                return;
            }
            if (recent_pid != 0)
            {
                printf("%u\n", recent_pid);
            }
            else
            {
                printf("No Recent processes found\n");
            }
            sleep(atoi(time_halt));
        }
    }
}