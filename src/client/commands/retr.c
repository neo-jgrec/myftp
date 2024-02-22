/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** retr
*/

#include "ftp.h"
#include <stdio.h>
#include <unistd.h>

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

static void retr_fork(FILE *file, int fd, client_t *client)
{
    char buffer[1024];
    size_t bytes;

    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    for (bytes = fread(buffer, 1, sizeof(buffer), file);
        bytes > 0; bytes = fread(buffer, 1, sizeof(buffer), file))
        tcp_send(fd, buffer, bytes);
    fclose(file);
    close(fd);
    close(client->data_fd);
    client->data_fd = -1;
    exit(0);
}

static int retr_passive(client_t *client, char *arg)
{
    FILE *file = fopen(arg, "rb");
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int fd;
    pid_t pid;

    if (!ERROR_HANDLING(file, "retr : fopen")) {
        dprintf(client->fd, "550 Failed to open file.\r\n");
        return 0;
    }
    tcp_send(client->fd, reply_start, strlen(reply_start));
    fd = accept(client->data_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (!ERROR_HANDLING(fd, "retr : accept"))
        return 1;
    pid = fork();
    if (pid == 0)
        retr_fork(file, fd, client);
    retr_passive_destructor(file, fd, client);
    return 0;
}

static int retr_port(client_t *client, char *arg)
{
    char *filename = arg;
    FILE *file = fopen(filename, "rb");
    pid_t pid;

    if (!ERROR_HANDLING(file, "retr : fopen")) {
        dprintf(client->fd, "550 Failed to open file.\r\n");
        return 0;
    }
    tcp_send(client->fd, reply_start, strlen(reply_start));
    pid = fork();
    if (!ERROR_HANDLING(pid, "retr : fork"))
        return 1;
    if (pid == 0)
        retr_fork(file, client->data_fd, client);
    fclose(file);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
    client->data_fd = -1;
    return 0;
}

int retr(client_t *client, char *arg)
{
    int ret = 0;

    if (client->transfer == PASSIVE_TRANSFER) {
        ret = retr_passive(client, arg);
        if (ret)
            dprintf(client->fd, "425 Can't open data connection.\r\n");
        return ret;
    } else if (client->transfer == ACTIVE_TRANSFER) {
        return retr_port(client, arg);
    } else {
        dprintf(client->fd, "425 Use PORT or PASV first.\r\n");
        return 1;
    }
}
