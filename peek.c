#include "headers.h"

//all done except ~ and -

void get_Directories(char* cmd, int hid, int info)
{
    struct dirent **dir;
    int curr_dir_no;
    char abs_path[1000];
    if(!(cmd[0] == '/')){
        getcwd(abs_path, sizeof(abs_path));
        strcat(abs_path, "/");
        strcat(abs_path, cmd);
    }
    else{
    strcpy(abs_path, cmd);
    }

    curr_dir_no = scandir(cmd, &dir, NULL, alphasort);
    if (curr_dir_no == -1)
    {
        perror("scandir");
        return;
    }
    for (int l = 0; l < curr_dir_no; l++)
    {
        if (dir[l]->d_name[0] != '.' || hid == 1)
        {
            struct stat space;
            char path[1000];
            strcpy(path, abs_path);
            strcat(path, "/");
            strcat(path, dir[l]->d_name);
            if (stat(path, &space) != -1)
            {
                char perms[12];
                perms[0] = (S_ISDIR(space.st_mode)) ? 'd' : '-';
                perms[1] = (space.st_mode & S_IRUSR) ? 'r' : '-';
                perms[2] = (space.st_mode & S_IWUSR) ? 'w' : '-';
                perms[3] = (space.st_mode & S_IXUSR) ? 'x' : '-';
                perms[4] = (space.st_mode & S_IRGRP) ? 'r' : '-';
                perms[5] = (space.st_mode & S_IWGRP) ? 'w' : '-';
                perms[6] = (space.st_mode & S_IXGRP) ? 'x' : '-';
                perms[7] = (space.st_mode & S_IROTH) ? 'r' : '-';
                perms[8] = (space.st_mode & S_IWOTH) ? 'w' : '-';
                perms[9] = (space.st_mode & S_IXOTH) ? 'x' : '-';
                perms[10] = '\0';
                struct passwd *pw = getpwuid(space.st_uid);
                struct group *gr = getgrgid(space.st_gid);
                struct tm *time_info = localtime(&space.st_mtime);
                char date_time[80];
                strftime(date_time, sizeof(date_time), "%b %d %H:%M", time_info);
                if (S_ISDIR(space.st_mode))
                {
                    if(info == 1){
                        printf("\e[36m%s %3ld %s %s %8ld %s %s\e[m\n", perms, (long int)space.st_nlink, pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown", space.st_size, date_time,  dir[l]->d_name);
                    }
                    else{

                    printf("\e[36m%s\e[m\n", dir[l]->d_name);
                    }
                }
                else if (S_IXUSR & space.st_mode)
                {
                    if(info == 1){
                        printf("\e[32m%s %3ld %s %s %8ld %s %s\e[m\n", perms, (long int)space.st_nlink, pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown", space.st_size , date_time, dir[l]->d_name);
                    }
                    else{
                    printf("\e[32m%s\e[m\n", dir[l]->d_name);
                    }
                }
                else
                {
                    if(info == 1){
                        printf("%s %3ld %s %s %8ld %s %s\n", perms, (long int)space.st_nlink, pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown", space.st_size, date_time, dir[l]->d_name);
                    }
                    else{
                    printf("%s\n", dir[l]->d_name);
                    }
                }
            }
        }
        free(dir[l]);
    }
    free(dir);
}

int peek(char **command, int bg, int cmd_len,int* pipe_fd,int* pipe_fd2,  int active_pipe)
{
    int hid = 0;
    int info = 0;
    char cmd[100];
    getcwd(cmd, sizeof(cmd));
    if (cmd_len == 1)
    {
        get_Directories(cmd, 0, 0);
    }
    else if (cmd_len == 2)
    {
        if (!strcmp(command[1], "-a") || !strcmp(command[1], "-al") || !strcmp(command[1], "-la"))
        {
            hid = 1;
        }
        if (!strcmp(command[1], "-l") || !strcmp(command[1], "-al") || !strcmp(command[1], "-la"))
        {
            info = 1;
        }

        if (!strcmp(command[1], "."))
        {
            get_Directories(cmd, 0, 0);
        }
        else if(!strcmp(command[1], "~")){
            get_Directories(gethome_len(), 0, 0);
        }
        else if (!strcmp(command[1], ".."))
        {
            char *temp = strrchr(cmd, '/');
            *temp = '\0';
            get_Directories(cmd, 0, 0);
        }
        else if (hid || info)
        {
            get_Directories(cmd, hid, info);
        }
        else
        {
            get_Directories(command[1], 0, 0);
        }
    }
    else if (cmd_len == 3)
    {
        if (!strcmp(command[1], "-a") || !strcmp(command[1], "-al") || !strcmp(command[1], "-la") || !strcmp(command[2], "-a"))
        {
            hid = 1;
        }
        if (!strcmp(command[1], "-l") || !strcmp(command[1], "-al") || !strcmp(command[1], "-la") || !strcmp(command[2], "-l"))
        {
            info = 1;
        }

        if (!strcmp(command[2], "."))
        {
            get_Directories(cmd, hid, info);
        }
        else if (!strcmp(command[2], "~"))
        {
            get_Directories(gethome_len(), hid, info);
        }
        else if (!strcmp(command[2], ".."))
        {
            char *temp = strrchr(cmd, '/');
            *temp = '\0';
            get_Directories(cmd, hid, info);
        }
        else if (hid && info)
        {
            if (strlen(command[1]) == 3)
            {
                get_Directories(command[2], hid, info);
            }
            else
            {
                get_Directories(cmd, hid, info);
            }
        }
        else if (hid || info)
        {
            get_Directories(command[2], hid, info);
        }
    }
    else if (cmd_len == 4)
    {
        if (!strcmp(command[1], "-a") || !strcmp(command[1], "-al") || !strcmp(command[1], "-la") || !strcmp(command[2], "-a"))
        {
            hid = 1;
        }
        if (!strcmp(command[1], "-l") || !strcmp(command[1], "-al") || !strcmp(command[1], "-la") || !strcmp(command[2], "-l"))
        {
            info = 1;
        }
        get_Directories(command[3], hid, info);
    }
    return 0;
}