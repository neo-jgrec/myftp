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
        return (dprintf(client->fd,
            "501 Syntax error in parameters or arguments.\r\n"), 1);
    client->username = strdup(arg);
    dprintf(client->fd, "331 User name okay, need password.\r\n");
    return 0;
}
