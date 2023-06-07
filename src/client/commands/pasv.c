/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pasv
*/

#include "ftp.h"

int pasv(client_t *client, char *arg)
{
    (void)arg;
    int port = 0;
    char *ip = get_ip();

    if (!error_handling(ip, "Failed to get ip"))
        return (EXIT_FAILURE);
    port = tcp_listen(0, SOMAXCONN);
    if (!error_handling(port, "Failed to create server socket"))
        return (EXIT_FAILURE);
    client->data_port = port;
    client->transfer = PASSIVE_TRANSFER;
    dprintf(client->fd, "227 Entering Passive Mode (%s,%d,%d)\r\n",
        ip, port / 256, port % 256);
    return (EXIT_SUCCESS);
}
