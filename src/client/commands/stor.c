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

static void handle_data_fd(client_t *client, FILE *file)
{
    char buffer[1024];
    ssize_t bytes = tcp_recv(client->data_fd, buffer, sizeof(buffer));

    while (bytes > 0) {
        fwrite(buffer, 1, bytes, file);
        bytes = tcp_recv(client->data_fd, buffer, sizeof(buffer));
    }
    fclose(file);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
}

static int stor_select(client_t *client, FILE *file)
{
    fd_set read_fds;
    int max_fd;
    int ready;

    tcp_fd_set_init(&read_fds, client->data_fd);
    max_fd = client->data_fd + 1;
    ready = tcp_select(max_fd, &read_fds);
    if (!ERROR_HANDLING(ready, "stor : select")) {
        fclose(file);
        return 1;
    }
    if (FD_ISSET(client->data_fd, &read_fds))
        handle_data_fd(client, file);
    return 0;
}

static int stor_active(client_t *client, char *arg)
{
    char *filename = arg;
    FILE *file = fopen(filename, "wb");

    if (!ERROR_HANDLING(file, "stor : fopen"))
        return 1;
    tcp_send(client->fd, reply_start, strlen(reply_start));
    if (stor_select(client, file))
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
    if (!ERROR_HANDLING(client->data_fd, "stor : accept")) {
        fclose(file);
        return 1;
    }
    if (stor_select(client, file))
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
