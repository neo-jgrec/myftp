/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pwd
*/

#include "ftp.h"

int pwd(client_t *client, char *arg)
{
    (void)arg;
    dprintf(client->fd, "257 \"%s\"\r\n", client->cwd);
    return (EXIT_SUCCESS);
}
