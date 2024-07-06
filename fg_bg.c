#include "headers.h"
void fg(char** command,int bg,int cmd_count,int* pipe_fd,int* pipe_fd2,int* i,int active_pipe) {
    int pid = atoi(command[1]);
    int fg_present;
    if (pid >= 0) {
        tcsetpgrp(STDIN_FILENO, pid);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU,SIG_IGN);
        if (kill(pid, SIGCONT) == 0) {
            printf("Bringing process [%d] to foreground.\n", pid);
            *i--;
            int status;
            fg_present = pid;
            waitpid(pid, &status, WUNTRACED);
            fg_present = 0;
        } else {
            perror("fg");
            exit(0);

    }
    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN,SIG_DFL);
    signal(SIGTTOU,SIG_DFL);
    }
     else {
        printf("No such process found.\n");
    }
}

void bg_func(char** command,int bg,int cmd_count,int* pipe_fd,int* pipe_fd2,  int active_pipe) {
    int pid = atoi(command[1]);
    if (pid >= 0) {
        if (kill(pid, SIGCONT) == 0) {
            printf("Resuming process [%d] in the background.\n", pid);
        } else {
            perror("bg");
        } 
    }
}