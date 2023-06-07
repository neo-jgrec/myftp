/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** cdup
*/

#include "ftp.h"

int cdup(client_t *client, char *arg)
{
    (void)arg;
    if (chdir("..") == -1)
        return (dprintf(client->fd, "xxx Error (RFC compliant)\r\n"), 1);
    if (client->cwd)
        free(client->cwd);
    client->cwd = getcwd(NULL, 0);
    return (dprintf(client->fd,
        "250 Requested file action okay, completed.\r\n"), 0);
}
