/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** list
*/

#include "ftp.h"

static const char *reply_start = "150 Here comes the directory listing.\r\n";
static const char *reply_complete = "226 Directory send OK.\r\n";

int list(client_t *client, char *arg)
{
    char *command = NULL;
    FILE *pipe = NULL;
    char buffer[4096];

    tcp_send(client->fd, reply_start, strlen(reply_start));
    asprintf(&command, "/bin/ls -l %s", (arg) ? arg : "");
    pipe = popen(command, "r");
    if (!error_handling(pipe, "popen listing"))
        return 1;
    while (!feof(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
            tcp_send(client->data_fd, buffer, strlen(buffer));
    }
    pclose(pipe);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
    client->data_fd = -1;
    free(command);
    return 0;
}
