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
    ssize_t bytes;

    while ((bytes = tcp_recv(client->data_fd, buffer, sizeof(buffer))) > 0)
        fwrite(buffer, 1, bytes, file);
    fclose(file);
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
    exit(0);
}

int stor(client_t *client, char *arg)
{
    char *filename = arg;
    FILE *file = fopen(filename, "wb");

    if (!error_handling(file, "stor : fopen"))
        return 1;
    tcp_send(client->fd, reply_start, strlen(reply_start));
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
