/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** port
*/

#include "ftp.h"

int port(client_t *client, char *arg)
{
    char *ip = NULL;
    int port = 0;

    (void)ip;
    if (!error_handling(arg, "Missing argument"))
        return (EXIT_FAILURE);
    ip = strtok(arg, ",");
    for (int i = 0; i < 4; i++)
        ip = strtok(NULL, ",");
    port = atoi(strtok(NULL, ",")) * 256;
    port += atoi(strtok(NULL, ","));
    client->data_port = port;
    client->transfer = ACTIVE_TRANSFER;
    dprintf(client->fd, "200 PORT command successful.\r\n");
    return (EXIT_SUCCESS);
}
