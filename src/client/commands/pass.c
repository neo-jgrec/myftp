/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pass
*/

#include "ftp.h"

int pass(client_t *client, char *arg)
{
    if (client->username == NULL) {
        dprintf(client->fd, "530 Please login with USER and PASS.\r\n");
        return 0;
    }
    if (arg == NULL) {
        dprintf(client->fd, "530 Permission denied.\r\n");
        return 0;
    }
    client->password = strdup(arg);
    dprintf(client->fd, "230 User logged in, proceed.\r\n");
    return 0;
}
