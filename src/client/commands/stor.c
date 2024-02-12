/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** stor
*/

#include "ftp.h"

static const char *reply_start =
"150 Opening BINARY mode data connection for file transfer.\r\n";
static const char *reply_complete = "226 Transfer complete.\r\n";

static void do_stor(client_t *client, FILE *file)
{
    char buffer[1024];
    ssize_t bytes = tcp_recv(client->data_fd, buffer, sizeof(buffer));

    for (; bytes > 0;
        bytes = tcp_recv(client->data_fd, buffer, sizeof(buffer)))
        fwrite(buffer, 1, bytes, file);
    fclose(file);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
    exit(0);
}

static int stor_fork(client_t *client, FILE *file)
{
    pid_t pid = fork();

    if (pid == -1) {
        fclose(file);
        return 1;
    } else if (pid > 0) {
        fclose(file);
        return 0;
    } else
        do_stor(client, file);
    return 0;
}

static int stor_active(client_t *client, char *arg)
{
    char *filename = arg;
    FILE *file = fopen(filename, "wb");

    if (!ERROR_HANDLING(file, "stor : fopen"))
        return 1;
    tcp_send(client->fd, reply_start, strlen(reply_start));
    if (stor_fork(client, file))
        return 1;
    return 0;
}

static int stor_passive(client_t *client, char *arg)
{
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char *filename = arg;
    FILE *file = fopen(filename, "wb");

    if (!ERROR_HANDLING(file, "stor : fopen"))
        return 1;
    tcp_send(client->fd, reply_start, strlen(reply_start));
    client->data_fd = accept(client->data_fd,
        (struct sockaddr *)&client_addr, &addrlen);
    if (client->data_fd == -1) {
        fclose(file);
        return 1;
    }
    if (stor_fork(client, file))
        return 1;
    return 0;
}

int stor(client_t *client, char *arg)
{
    if (client->transfer == ACTIVE_TRANSFER)
        return stor_active(client, arg);
    else if (client->transfer == PASSIVE_TRANSFER)
        return stor_passive(client, arg);
    return 0;
}
