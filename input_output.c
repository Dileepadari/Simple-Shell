#include "headers.h"

void in_out_redirection(char **command, int bg, int cmd_len, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    char *input_file = NULL;
    char *output_file = NULL;
    int input_fd = -1;
    int output_fd = -1;

    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        for (int i = 0; i < cmd_len; i++)
        {
            if (strcmp(command[i], "<") == 0)
            {
                command[i] = NULL;
                if (i + 1 < cmd_len)
                {
                    input_file = command[i + 1];
                    input_fd = open(input_file, O_RDONLY);
                    if (input_fd == -1)
                    {
                        perror("open");
                        printf("No such input file found: %s\n", input_file);
                        return;
                    }
                }
            }
            else if (strcmp(command[i], ">") == 0)
            {
                command[i] = NULL;
                if (i + 1 < cmd_len)
                {
                    output_file = command[i + 1];
                    output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (output_fd == -1)
                    {
                        perror("open");
                        return;
                    }
                }
            }
            else if (strcmp(command[i], ">>") == 0)
            {
                command[i] = NULL;
                if (i + 1 < cmd_len)
                {
                    output_file = command[i + 1];
                    output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (output_fd == -1)
                    {
                        perror("open");
                        return;
                    }
                }
            }
        }
        if (input_fd != -1)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != -1)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        int err = execvp(command[0], command);
        if (err == -1)
        {
            printf("ERROR : '%s' is not a valid command\n", command[0]);
        }
    }
    else
    {
        fg_present = pid;
        waitpid(pid, NULL, 0);
        fg_present = 0;
    }
}