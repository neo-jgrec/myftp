/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** help
*/

#include "ftp.h"

int help(client_t *client, UNUSED char *arg)
{
    dprintf(client->fd, "214-The following commands are recognized.\r\n");
    for (int i = 0; commands[i].name != NULL; i++)
        dprintf(client->fd, "%s\r\n", commands[i].name);
    dprintf(client->fd, "214 Help OK.\r\n");
    return (EXIT_SUCCESS);
}
