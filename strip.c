#include "headers.h"

void lstrip(char *command)
{
    int len = strlen(command);
    int i = 0;

    while (isspace(command[i]))
    {
        i++;
    }

    if (i > 0)
    {
        memmove(command, command + i, len - i + 1);
    }
}

void rstrip(char *command)
{
    int len = strlen(command);

    while (len > 0 && isspace(command[len - 1]))
    {
        len--;
    }

    command[len] = '\0';
}