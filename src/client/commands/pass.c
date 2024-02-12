/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pass
*/

#include "ftp.h"

int pass(client_t *client, char *arg)
{
    int isValidPassword = 0;

    arg = arg ? arg : "";
    for (int i = 0; default_users[i].username != NULL; i++) {
        if (strcmp(default_users[i].username, client->username) == 0
            && strcmp(default_users[i].password, arg) == 0) {
            isValidPassword = 1;
            break;
        }
    }
    if (!isValidPassword)
        return dprintf(client->fd, "530 Login incorrect.\r\n"), -1;
    if (client->password)
        free(client->password);
    client->password = strdup(arg);
    return dprintf(client->fd, "230 User logged in, proceed.\r\n"), 0;
}
