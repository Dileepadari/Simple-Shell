#include "headers.h"

void ctrlc_handle(int sig)
{
    int fg_present;
    if (fg_present != 0)
    {
        if (kill(fg_present, SIGKILL) != 0)
        {
        }
    }
}
void seg_handle(int sig)
{
    take_input();
}
void ctrlD_handle(int sig)
{
    if (fg_present != 0)
    {
        if (kill(fg_present, SIGKILL) != 0)
        {
        }
    }
}
void ctrlz_handle(int seg)
{
    int fg_present;
    if (fg_present != 0)
    {
        printf("Sending process to background");
        if (kill(fg_present, SIGSTOP) != 0)
        {
        }
    }
}
void send_signal(char **command, int bg, int cmd_len, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    pid_t pid = atoi(command[1]);
    int signalNumber = atoi(command[2]) % 32;
    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
        exit(1);
    }
    if (kill(pid, signalNumber) == -1)
    {
        perror("Failed to send signal");
        exit(1);
    }

    printf("Sent signal %d to process with PID %d\n", signalNumber, pid);
}
