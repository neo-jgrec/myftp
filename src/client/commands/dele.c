/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** dele
*/

#include "ftp.h"

int dele(client_t *client, char *arg)
{
    char *path = NULL;
    char *buffer = NULL;

    if (!ERROR_HANDLING(arg, "Missing argument"))
        return (EXIT_FAILURE);
    path = get_path(client->cwd, arg);
    if (!ERROR_HANDLING(path, "Invalid path"))
        return (EXIT_FAILURE);
    if (remove(path) == -1) {
        buffer = strerror(errno);
        dprintf(client->fd, "550 %s\r\n", buffer);
        return (EXIT_FAILURE);
    }
    dprintf(client->fd, "250 Requested file action okay, completed.\r\n");
    return (EXIT_SUCCESS);
}
