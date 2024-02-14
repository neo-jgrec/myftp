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
    char reply[] = "250 Requested file action okay, completed.\r\n";

    if (client->username == NULL) {
        dprintf(client->fd, "530 Please login with USER and PASS.\r\n");
        return 0;
    }
    if (arg == NULL) {
        dprintf(client->fd, "550 Failed to delete file.\r\n");
        return 0;
    }
    asprintf(&path, "rm -f %s", arg);
    if (system(path) == -1) {
        dprintf(client->fd, "550 Failed to delete file.\r\n");
        free(path);
        return 0;
    }
    dprintf(client->fd, "%s", reply);
    free(path);
    return 0;
}
