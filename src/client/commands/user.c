/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** user
*/

#include "ftp.h"

int user(client_t *client, char *arg)
{
    if (!arg)
        return (dprintf(client->fd, "xxx Error (RFC compliant)\r\n"), 1);
    if (client->username)
        free(client->username);
    client->username = strdup(arg);
    return (dprintf(client->fd, "331 User name okay, need password.\r\n"), 0);
}
