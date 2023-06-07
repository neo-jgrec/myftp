/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pass
*/

#include "ftp.h"

int pass(client_t *client, char *arg)
{
    arg = arg ? arg : "";
    if (client->password)
        free(client->password);
    client->password = strdup(arg);
    return (dprintf(client->fd, "230 User logged in, proceed.\r\n"), 0);
}
