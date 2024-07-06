#include "headers.h"
int fg_present;
int prev_prom = 0;
int ps_no = 0;
char prev_prom_msg[255];
char bg_prom_msg[255];

void verify(char *input)
{
    save_to_pastevents(input);
    char *Total_entry_parts[100];
    char *Total_commands[100];
    char *piping_cmd[100];
    int counter = 0;
    int IO_flag;
    Total_entry_parts[counter] = strtok(input, ";");
    while (Total_entry_parts[counter] != NULL)
    {
        counter++;
        Total_entry_parts[counter] = strtok(NULL, ";");
    }
    int count = 0;
    int bg[100];
    for (int i = 0; i < counter; i++)
    {
        Total_commands[count] = strtok(Total_entry_parts[i], "&");
        while (Total_commands[count] != NULL)
        {
            bg[count] = 1;
            count++;
            Total_commands[count] = strtok(NULL, "&");
        }
        bg[count - 1] = 0;
    }
    for (int i = 0; i < count; i++)
    {
        if (!strcmp(Total_commands[i], "\n") || !strcmp(Total_commands[i], ""))
        {
            continue;
        }
        int pipe_count = 0;
        piping_cmd[pipe_count] = strtok(Total_commands[i], "|");
        while (piping_cmd[pipe_count] != NULL)
        {
            lstrip(piping_cmd[pipe_count]);
            rstrip(piping_cmd[pipe_count]);
            if (!strcmp(piping_cmd[pipe_count], ""))
            {
                printf("Invalid Use of pipe\n");
                return;
            }
            pipe_count++;
            piping_cmd[pipe_count] = strtok(NULL, "|");
        }

        for (int j = 0; j < pipe_count; j++)
        {
            int active_pipe = 0;
            char *command_test[100] = {'\0'};
            char *command[100] = {'\0'};
            int cmd_count = 0;
            int cmd_test_Count = 0;
            char delimeters_test[] = "'\'''\"''\n'";
            char *dummy;

            command_test[cmd_test_Count] = strtok(piping_cmd[j], delimeters_test);
            while (command_test[cmd_test_Count] != NULL)
            {
                lstrip(command_test[cmd_test_Count]);
                rstrip(command_test[cmd_test_Count]);
                cmd_test_Count++;
                command_test[cmd_test_Count] = strtok(NULL, delimeters_test);
            }
            char delimeters[] = "'\t'' '";
            for (int k = 0; k < cmd_test_Count; k++)
            {
                lstrip(command_test[k]);
                rstrip(command_test[k]);
                if (k == 1 || (strstr(command_test[k], "seek") > 0 && strstr(command_test[k], "warp") > 0 && strstr(command_test[k], "/") > 0))
                {
                    command[cmd_count] = command_test[k];
                    cmd_count++;
                    continue;
                }
                IO_flag = 0;
                command[cmd_count] = strtok(command_test[k], delimeters);
                while (command[cmd_count] != NULL)
                {
                    lstrip(command[cmd_count]);
                    rstrip(command[cmd_count]);
                    if (!(strcmp(command[cmd_count], "<")) || (!strcmp(command[cmd_count], ">")) || (!strcmp(command[cmd_count], ">>")))
                    {
                        IO_flag = 1;
                    }
                    cmd_count++;
                    command[cmd_count] = strtok(NULL, delimeters);
                }
            }
            int pipe_fd[2];
            int pipe_fd2[2];
            if (pipe_count > 1)
            {
                if (j % 2 != 0)
                {
                    pipe(pipe_fd);
                }
                else
                {
                    pipe(pipe_fd2);
                }
            }
            if (command[0])
            {

                if (!strcmp(command[0], "warp"))
                {
                    warp(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
                }
                else if (!strcmp(command[0], "fg"))
                {
                    fg(command, bg[i], cmd_count, pipe_fd, pipe_fd2, &ps_no, active_pipe);
                }
                else if (!strcmp(command[0], "activities"))
                {
                    activities();
                }
                else if (!strcmp(command[0], "seek"))
                {
                    seek_flags(command, bg[i], cmd_count, pipe_fd, pipe_fd2, active_pipe);
                }
                else
                {

                    execvp_func(command, bg[i], cmd_count, pipe_count, &prev_prom, prev_prom_msg, bg_prom_msg, &ps_no, pipe_fd, pipe_fd2, j, IO_flag);
                }
            }
        }
    }
}

void take_input()
{
    fg_present = 0;
    prompt(&prev_prom, prev_prom_msg);
    char input[4096] = {'\0'};
    if (fgets(input, 4096, stdin) == NULL)
    {
        printf("\nCtrl+D detected. Exiting the shell.\n");
        exit(0);
    }
    verify(input);
}

int main()
{
    signal(SIGINT, ctrlc_handle);
    signal(SIGTSTP, ctrlz_handle);
    while (1)
    {
        fg_present = 0;

        int normality;
        pid_t completed_pid = waitpid(-1, &normality, WNOHANG);
        while (completed_pid > 0)
        {
            char norm[20];
            if (WIFEXITED(normality))
            {
                strcpy(norm, "normally");
            }
            else
            {
                strcpy(norm, "abnormally");
            }
            ps_no--;
            printf("Process with pid %d exited %s\n", completed_pid, norm);
            completed_pid = waitpid(-1, &normality, WNOHANG);
        }
        take_input();
    }
}
