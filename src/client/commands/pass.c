/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pass
*/

#include "ftp.h"

int pass(client_t *client, char *arg)
{
    if (!arg)
        return (dprintf(client->fd, "xxx Error (RFC compliant)\r\n"), 1);
    if (client->password)
        free(client->password);
    client->password = strdup(arg);
    return (dprintf(client->fd, "230 User logged in, proceed.\r\n"), 0);
}
