#include "headers.h"
int dir_count = 0;
int file_count = 0;
char req_path[200];
void seek(char *req_dir_fil, char *target_dir, int is_direct, int is_file, int is_task, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    if (!strcmp(target_dir, "."))
    {
        getcwd(target_dir, sizeof(target_dir));
    }
    DIR *dir = opendir(target_dir);
    struct dirent *direc;
    while ((direc = readdir(dir)))
    {
        if (direc->d_name[0] != '.')
        {
            char path[1000];
            struct stat space;
            snprintf(path, sizeof(path), "%s/%s", target_dir, direc->d_name);
            char *temp = strtok(direc->d_name, ".");
            if ((!strcmp(direc->d_name, req_dir_fil)) || (!(strcmp(temp, req_dir_fil))))
            {
                if (stat(path, &space) == 0 && S_ISDIR(space.st_mode))
                {
                    if (is_direct || !(is_file))
                    {
                        dir_count++;
                        strcpy(req_path, path);
                        printf("\e[36m%s\e[m\n", path);
                    }
                }
                else
                {
                    if (!(is_direct) || is_file)
                    {
                        file_count++;
                        strcpy(req_path, path);
                        printf("\e[32m%s\e[m\n", path);
                    }
                }
            }
            if (stat(path, &space) == 0 && S_ISDIR(space.st_mode))
            {
                seek(req_dir_fil, path, is_direct, is_file, is_task, pipe_fd, pipe_fd2, active_pipe);
            }
        }
    }
    closedir(dir);
}

void seek_flags(char **command, int bg, int cmd_count, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    file_count = 0;
    dir_count = 0;
    int is_direct = 0;
    int is_file = 0;
    int is_task = 0;
    if (cmd_count < 2)
    {
        printf("Error: No directory specified.\n");
        return;
    }
    if ((cmd_count >= 2 && !strcmp(command[1], "-d")) || (cmd_count >= 3 && !strcmp(command[2], "-d")))
    {
        is_direct = 1;
    }
    if ((cmd_count >= 2 && !strcmp(command[1], "-f")) || (cmd_count >= 3 && !strcmp(command[2], "-f")))
    {
        is_file = 1;
    }
    if ((cmd_count >= 2 && !strcmp(command[1], "-e")) || (cmd_count >= 3 && !strcmp(command[2], "-e")))
    {
        is_task = 1;
    }
    if (is_direct + is_file + is_task == 0)
    {
        if (cmd_count == 2)
        {
            command[2] = (char *)malloc(100 * sizeof(char));
            strcpy(command[2], ".");
        }
        seek(command[1], command[2], is_direct, is_file, is_task, pipe_fd, pipe_fd2, active_pipe);
    }
    else if (is_direct + is_file + is_task == 1)
    {
        if (cmd_count == 3)
        {
            command[3] = (char *)malloc(100 * sizeof(char));
            strcpy(command[3], ".");
        }
        seek(command[2], command[3], is_direct, is_file, is_task, pipe_fd, pipe_fd2, active_pipe);
    }
    else if (is_direct + is_file + is_task == 2)
    {
        if (is_direct && is_file)
        {
            printf("Invalid flags!\n");
            return;
        }
        if (cmd_count == 4)
        {
            command[4] = (char *)malloc(100 * sizeof(char));
            strcpy(command[4], ".");
        }
        seek(command[3], command[4], is_direct, is_file, is_task, pipe_fd, pipe_fd2, active_pipe);
    }
    if (is_task & (file_count == 1) && (dir_count == 0))
    {
        FILE *file = fopen(req_path, "r");
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file))
        {
            printf("%s", buffer);
        }
        printf("\n");
        fclose(file);
    }
    else if (is_task & (file_count == 0) && (dir_count == 1))
    {
        if (chdir(req_path))
        {
            printf("Missing permissions for task!\n");
        }
    }
    if (file_count == 0 && dir_count == 0)
    {
        printf("No match found\n");
    }
}