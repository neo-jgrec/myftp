/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** retr
*/

#include "ftp.h"
#include <stdio.h>

static const char *reply_start =
"150 Opening BINARY mode data connection for file transfer.\r\n";
static const char *reply_complete = "226 Transfer complete.\r\n";

static void retr_passive_destructor(FILE* file, int fd, client_t *client)
{
    fclose(file);
    close(fd);
    close(client->data_fd);
    client->data_fd = -1;
}

static int retr_passive(client_t *client, char *arg)
{
    FILE *file = fopen(arg, "rb");
    char buffer[1024];
    size_t bytes;
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int fd;

    if (!ERROR_HANDLING(file, "retr : fopen"))
        return 1;
    tcp_send(client->fd, reply_start, strlen(reply_start));
    fd = accept(client->data_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (fd == -1)
        return 1;
    for (bytes = fread(buffer, 1, sizeof(buffer), file);
        bytes > 0; bytes = fread(buffer, 1, sizeof(buffer), file))
        tcp_send(fd, buffer, bytes);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    retr_passive_destructor(file, fd, client);
    return 0;
}

static int retr_port(client_t *client, char *arg)
{
    char *filename = arg;
    FILE *file = fopen(filename, "rb");
    char buffer[1024];
    size_t bytes;

    if (!ERROR_HANDLING(file, "retr : fopen"))
        return 1;
    tcp_send(client->fd, reply_start, strlen(reply_start));
    for (bytes = fread(buffer, 1, sizeof(buffer), file);
        bytes > 0; bytes = fread(buffer, 1, sizeof(buffer), file))
        tcp_send(client->data_fd, buffer, bytes);
    fclose(file);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
    client->data_fd = -1;
    return 0;
}

int retr(client_t *client, char *arg)
{
    if (client->transfer == PASSIVE_TRANSFER) {
        return retr_passive(client, arg);
    } else if (client->transfer == ACTIVE_TRANSFER) {
        return retr_port(client, arg);
    } else {
        return 1;
    }
}
