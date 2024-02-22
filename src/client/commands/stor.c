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

static void stor_fork(client_t *client, FILE *file)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("stor : fork");
        fclose(file);
        return;
    }
    if (pid == 0) {
        handle_data_fd(client, file);
        exit(0);
    }
    fclose(file);
}

static int stor_active(client_t *client, char *arg)
{
    char *filename = arg;
    FILE *file = fopen(filename, "wb");

    if (connect(client->data_fd, (struct sockaddr *)&client->data_addr,
        sizeof(client->data_addr)) == -1) {
        dprintf(client->fd, "425 Can't open data connection.\r\n");
        return 1;
    }
    if (!ERROR_HANDLING(file, "stor : fopen")) {
        dprintf(client->fd, "550 Failed to open file.\r\n");
        return 1;
    }
    tcp_send(client->fd, reply_start, strlen(reply_start));
    stor_fork(client, file);
    return 0;
}

static int stor_passive(client_t *client, char *arg)
{
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char *filename = arg;
    FILE *file = fopen(filename, "wb");

    if (!ERROR_HANDLING(file, "stor : fopen")) {
        dprintf(client->fd, "550 Failed to open file.\r\n");
        return 1;
    }
    tcp_send(client->fd, reply_start, strlen(reply_start));
    client->data_fd = accept(client->data_fd,
        (struct sockaddr *)&client_addr, &addrlen);
    if (!ERROR_HANDLING(client->data_fd, "stor : accept")) {
        fclose(file);
        dprintf(client->fd, "425 Can't open data connection.\r\n");
        return 1;
    }
    stor_fork(client, file);
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
