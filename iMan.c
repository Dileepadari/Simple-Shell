#include "headers.h"

void iMan(char **command, int bg, int cmd_count, int *pipe_fd, int *pipe_fd2, int active_pipe)
{
    struct hostent *web = gethostbyname("man.he.net");
    if (web != NULL)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock != -1)
        {
            struct sockaddr_in web_address;
            struct in_addr **addr_list = (struct in_addr **)web->h_addr_list;
            web_address.sin_addr = *addr_list[0];
            web_address.sin_family = AF_INET;
            web_address.sin_port = htons(80);

            int connection = connect(sock, (struct sockaddr *)&web_address, sizeof(web_address));
            if (connection != -1)
            {
                char requested_content[20480];
                snprintf(requested_content, sizeof(requested_content), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command[1]);
                int request = send(sock, requested_content, strlen(requested_content), 0);
                if (request != -1)
                {
                    char manual[40960];
                    int result = recv(sock, manual, sizeof(manual) - 1, 0);
                    if (result > 0)
                    {
                        manual[result] = '\0';
                        char *newline = strchr(manual, '\n');
                        (newline)++;
                        int i;
                        for (i = 0; newline != NULL && i <= 30; i++)
                        {
                            if (!strncmp(newline, "NAME", 4))
                            {
                                break;
                            }
                            newline = strchr(newline, '\n');
                            if (newline != NULL)
                            {
                                (newline)++;
                            }
                            else
                            {
                                printf("Error:\n\tNo Such Command %s\n", command[1]);
                                return;
                            }
                        }
                        if (i == 31 || newline == NULL)
                        {
                            printf("Error:\n\t\tNo Such Command %s\n", command[1]);
                            return;
                        }
                        char *lastlines = strchr(newline, '\n');
                        (lastlines)++;
                        while (lastlines != NULL)
                        {
                            if (!strncmp(lastlines, "SEE ALSO", strlen("SEE ALSO")))
                            {
                                break;
                            }
                            lastlines = strchr(lastlines, '\n');
                            if (lastlines != NULL)
                            {
                                (lastlines)++;
                            }
                            else
                            {

                                break;
                            }
                        }
                        if (lastlines != NULL)
                        {
                            lastlines = strchr(lastlines, '\n');
                            (lastlines)++;
                            lastlines = strchr(lastlines, '\n');
                            (lastlines)++;
                            lastlines = strchr(lastlines, '\n');
                            (lastlines)++;
                            (*lastlines) = '\0';
                        }
                        printf("%s\n", newline);
                        printf("\n\n");
                    }
                    else
                    {
                        perror("failed getting manual");
                    }
                }
                else
                {
                    perror("Request not send");
                }
            }
            else
            {
                perror("Connection failed");
            }
        }
        else
        {
            perror("failed to create a socket");
        }
    }
    else
    {
        perror("DNS failed");
    }
}