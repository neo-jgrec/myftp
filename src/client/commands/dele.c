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
    int ret = 0;

    if (arg == NULL) {
        dprintf(client->fd, "550 Failed to delete file.\r\n");
        return 0;
    }
    asprintf(&path, "%s/%s", getcwd(NULL, 0), arg);
    ret = remove(path);
    if (ret == 0) {
        tcp_send(client->fd, reply, strlen(reply));
    } else {
        dprintf(client->fd, "550 Failed to delete file.\r\n");
    }
    return 0;
}
