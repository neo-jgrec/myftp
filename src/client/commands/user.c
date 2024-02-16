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
    for (int i = 0; default_users[i].username != NULL; i++) {
        if (strcmp(arg, default_users[i].username) == 0) {
            client->username = strdup(arg);
            return (dprintf(client->fd,
                "331 User name okay, need password.\r\n"), 0);
        }
    }
    return (dprintf(client->fd, "530 Not logged in.\r\n"), 1);
}
