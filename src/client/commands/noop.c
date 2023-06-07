/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** noop
*/

#include "ftp.h"

int noop(client_t *client, UNUSED char *arg)
{
    dprintf(client->fd, "200 Command okay.\r\n");
    return (EXIT_SUCCESS);
}
