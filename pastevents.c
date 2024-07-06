#include "headers.h"

char histo_link[255];
int get_a_line(char **result)
{
    strcpy(histo_link, gethome_len());
    strcat(histo_link, "/");
    strcat(histo_link, "history.bin");
    int fd = open(histo_link, O_RDONLY | O_CREAT, 0644);
    char lines[1000] = {'\0'};

    read(fd, lines, sizeof(lines));
    result[0] = strtok(lines, "\n");
    int i = 0;
    for (i = 0; i < 15 && result[i]; i++)
    {
        result[i + 1] = strtok(NULL, "\n");
    }
    close(fd);
    return i;
}

void getfromfile(char *result)
{
    char *resulter[200] = {'\0'};
    get_a_line(resulter);
    char lines[1000] = {'\0'};
    int i = 0;
    while (resulter[i] && i < 15)
    {
        strcat(lines, resulter[i]);
        strcat(lines, "\n");
        i++;
    }

    strcpy(result, lines);
}

void get_first_line(char *first_line)
{
    char line[1000] = {'\0'};
    getfromfile(line);
    int i = 0;
    while (line[i] && line[i] != '\n')
    {
        first_line[i] = line[i];
        i++;
    }
    first_line[i] = '\n';
    first_line[i + 1] = '\0';
};

void save_to_pastevents(char *input)
{
    char result[2000] = {'\0'};
    strcpy(histo_link, gethome_len());
    strcat(histo_link, "/");
    strcat(histo_link, "history.bin");
    getfromfile(result);
    char first_line[200];
    get_first_line(first_line);
    int fd = open(histo_link, O_RDWR | O_CREAT | O_TRUNC, 0644);
    char temp[2000] = {'\0'};
    if (input[strlen(input) - 1] != '\n')
    {
        strcat(input, "\n");
    }
    strcpy(temp, input);
    if (strcmp(temp, first_line) && strcmp(temp, "\n") && strcmp(temp, " \n") && (!strstr(temp, "pastevents")))
    {
        strcat(temp, result);
        write(fd, temp, strlen(temp));
    }
    else
    {
        write(fd, result, strlen(result) - 1);
    }
    close(fd);
}

void pastevents(char **command, int bg, int cmd_len, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    strcpy(histo_link, gethome_len());
    strcat(histo_link, "/");
    strcat(histo_link, "history.bin");
    int fd = open(histo_link, O_RDWR | O_CREAT, 0644);
    char lines[1000] = {'\0'};

    read(fd, lines, sizeof(lines));
    if (cmd_len == 1)
    {
        char *result[2000];
        int num = get_a_line(result);

        int i = num - 1;
        while (i >= 0 && result[i])
        {
            printf("%s\n", result[i]);
            i--;
        }
    }
    else if (cmd_len == 2 && !strcmp(command[1], "purge"))
    {
        ftruncate(fd, 0);
        close(fd);
        memset(&lines, '\0', sizeof(lines));
        return;
    }
    else if (cmd_len == 3 && !strcmp(command[1], "execute"))
    {
        lseek(fd, 0, SEEK_CUR);
        char *result[200] = {'\0'};
        get_a_line(result);
        strcat(result[atoi(command[2]) - 1], "\n");
        verify(result[atoi(command[2]) - 1]);
    }
    close(fd);
}