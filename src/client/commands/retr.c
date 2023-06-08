/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** retr
*/

#include "ftp.h"

static const char *reply_start =
"150 Opening BINARY mode data connection for file transfer.\r\n";
static const char *reply_complete = "226 Transfer complete.\r\n";

int retr(client_t *client, char *arg)
{
    char *filename = arg;
    FILE *file = fopen(filename, "rb");
    char buffer[1024];
    size_t bytes;

    if (!error_handling(file, "retr : fopen"))
        return 1;
    tcp_send(client->fd, reply_start, strlen(reply_start));
    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
        tcp_send(client->data_fd, buffer, bytes);
    fclose(file);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
    client->data_fd = -1;
    return 0;
}
