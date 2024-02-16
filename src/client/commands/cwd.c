/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** cwd
*/

#include "ftp.h"

int cwd(client_t *client, char *arg)
{
    char *cwd = NULL;

    if (!arg)
        return (dprintf(client->fd, "550 Failed to change directory.\r\n"), 1);
    cwd = realpath(arg, NULL);
    if (!cwd)
        return (dprintf(client->fd, "550 Failed to change directory.\r\n"), 1);
    if (chdir(cwd) == -1)
        return (dprintf(client->fd, "550 Failed to change directory.\r\n"), 1);
    if (client->cwd)
        free(client->cwd);
    client->cwd = cwd;
    return (dprintf(client->fd,
        "250 Requested file action okay, completed.\r\n"), 0);
}
