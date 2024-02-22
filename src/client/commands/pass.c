/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pass
*/

#include "ftp.h"

#include <stdbool.h>
#include <string.h>

static bool check_password(const char *username, const char *password)
{
    for (int i = 0; default_users[i].username != NULL; i++) {
        if (strcmp(default_users[i].username, username) == 0 &&
            strcmp(default_users[i].password, password) == 0) {
            return true;
        }
    }
    return false;
}

int pass(client_t *client, char *arg)
{
    if (client->username == NULL) {
        dprintf(client->fd, "530 Please login with USER and PASS.\r\n");
        return 0;
    }
    arg = arg ? arg : "";
    client->password = strdup(arg);
    if (check_password(client->username, client->password)) {
        dprintf(client->fd, "230 User logged in, proceed.\r\n");
        client->logged_in = true;
    } else {
        dprintf(client->fd, "530 Login incorrect.\r\n");
    }
    return 0;
}
