#include "headers.h"

void execvp_func(char **command, int bg, int cmd_count, int pipe_count, int *prev_prom, char *prev_prom_msg, char *bg_prom_msg, int *i, int *pipe_fd, int *pipe_fd2, int active_pipe, int IO_flag)
{
    int neon;
    int pid = fork();
    if (pid < 0)
    {
        if (active_pipe != pipe_count - 1 && pipe_count > 1)
        {
            if (active_pipe % 2 != 0)
            {
                close(pipe_fd[1]);
            }
            else
            {
                close(pipe_fd2[1]); 
            }
        }
        perror("Fork failed");
    }
    else if (pid == 0)
    {
        if (bg)
        {
            setpgid(0, 0);
        }
        else if (active_pipe == 0 && pipe_count > 1)
        {
            dup2(pipe_fd2[1], STDOUT_FILENO);
        }
        else if (active_pipe == pipe_count - 1 && pipe_count > 1)
        {
            if (pipe_count % 2 != 0)
            { 
                dup2(pipe_fd[0], STDIN_FILENO);
            }
            else
            { 
                dup2(pipe_fd2[0], STDIN_FILENO);
            }
        }
        else if (pipe_count > 1)
        {
            if (active_pipe % 2 != 0)
            {
                dup2(pipe_fd2[0], STDIN_FILENO);
                dup2(pipe_fd[1], STDOUT_FILENO);
            }
            else
            { 
                dup2(pipe_fd[0], STDIN_FILENO);
                dup2(pipe_fd2[1], STDOUT_FILENO);
            }
        }
        if (command[0])
        {

            if (!strcmp(command[0], "neonate"))
            {
                if (command[1] && !strcmp(command[1], "-n"))
                {
                    neon = 1;
                    fg_present = 0;
                    neonate(command[2]);
                }
                else
                {
                    printf("Invalid Format for neonate ");
                    exit(0);
                }
            }
            else if (IO_flag == 1)
            {
                in_out_redirection(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
            }

            else if (!strcmp(command[0], "peek"))
            {
                // command similar to cd
                peek(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
            }
            else if (!strcmp(command[0], "bg"))
            {
                bg_func(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
            }
            else if (!strcmp(command[0], "ping"))
            {
                send_signal(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
            }
            else if (!strcmp(command[0], "proclore"))
            {
                proclore(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
            }

            else if (!strcmp(command[0], "iMan"))
            {
                iMan(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
            }
            else if (!strcmp(command[0], "pastevents"))
            {
                pastevents(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
            }
            else if (!strcmp(command[0], "exit"))
            {
                printf("Exited with code 0\n");
                kill(pid, SIGKILL);
                exit(EXIT_SUCCESS);
            }
            else
            {
                if (execvp(command[0], command) == -1)
                {
                    printf("ERROR : '%s' is not a valid command\n", command[0]);
                    kill(getpid(), SIGTERM);
                }
            }
            exit(1);
        }
    }
    else
    {
        if (!bg)
        {
            signal(SIGINT, ctrlc_handle);
            signal(SIGTSTP, ctrlz_handle);
            time_t start_time, end_time;
            time(&start_time);
            fg_present = pid;
            if (neon == 1)
            {
                fg_present = 0;
            }

            waitpid(pid, NULL, WUNTRACED);
            time(&end_time);
            int time_taken = difftime(end_time, start_time);

            if (time_taken > 2)
            {
                *prev_prom = 1;
                strcpy(prev_prom_msg, command[0]);
                strcat(prev_prom_msg, " : ");
                char time_took[32];
                sprintf(time_took, "%d", time_taken);
                strcat(prev_prom_msg, time_took);
            }
        }
        else
        {
            strcpy(prev_prom_msg, command[0]);
            printf("[%d] %d\n", ++(*i), pid);
        }

        if (active_pipe == 0 && pipe_count > 1)
        {
            close(pipe_fd2[1]);
        }
        else if (active_pipe == pipe_count - 1 && pipe_count > 1)
        {
            if (pipe_count % 2 != 0)
            {
                close(pipe_fd[0]);
            }
            else
            {
                close(pipe_fd2[0]);
            }
        }
        else if (pipe_count > 1)
        {
            if (active_pipe % 2 != 0)
            {
                close(pipe_fd2[0]);
                close(pipe_fd[1]);
            }
            else
            {
                close(pipe_fd[0]);
                close(pipe_fd2[1]);
            }
        }
    }
}