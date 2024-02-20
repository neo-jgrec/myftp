/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** quit
*/

#include "ftp.h"

int quit(client_t *client, char *arg)
{
    (void)arg;
    dprintf(client->fd, "221 Service closing control connection.\r\n");
    return (0);
}
