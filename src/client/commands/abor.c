/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** abor
*/

#include "ftp.h"

int abor(client_t *client, char *arg)
{
    char reply[] = "226 Closing data connection.\r\n";
    if (!error_handling((int)tcp_send(client->fd, reply,
        strlen(reply)), "abor : tcp_send"))
        return 1;
    close(client->data_fd);
    client->data_fd = -1;
    return 0;
}
