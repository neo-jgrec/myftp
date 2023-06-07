/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** dele
*/

#include "ftp.h"

static char *get_path(char *cwd, char *arg)
{
    char *path = NULL;

    if (arg[0] == '/')
        path = strdup(arg);
    else {
        path = malloc(sizeof(char) * (strlen(cwd) + strlen(arg) + 2));
        asprintf(&path, "%s/%s", cwd, arg);
    }
    return (path);
}

int dele(client_t *client, char *arg)
{
    char *path = NULL;
    char *buffer = NULL;

    if (!error_handling(arg, "Missing argument"))
        return (EXIT_FAILURE);
    path = get_path(client->cwd, arg);
    if (!error_handling(path, "Invalid path"))
        return (EXIT_FAILURE);
    if (remove(path) == -1) {
        buffer = strerror(errno);
        dprintf(client->fd, "550 %s\r\n", buffer);
        return (EXIT_FAILURE);
    }
    dprintf(client->fd, "250 Requested file action okay, completed.\r\n");
    return (EXIT_SUCCESS);
}
