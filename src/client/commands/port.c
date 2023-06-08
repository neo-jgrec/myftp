/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** port
*/

#include "ftp.h"

int port(client_t *client, char *arg)
{
    unsigned int h1, h2, h3, h4, p1, p2;
    sscanf(arg, "%u,%u,%u,%u,%u,%u", &h1, &h2, &h3, &h4, &p1, &p2);

    unsigned short port = p1 * 256 + p2;
    char ip[16];
    snprintf(ip, 16, "%u.%u.%u.%u", h1, h2, h3, h4);

    int data_socket = tcp_connect(ip, port);
    if (!error_handling(data_socket, "port : tcp_connect"))
        return 1;

    client->transfer = ACTIVE_TRANSFER;
    client->data_port = port;
    client->data_fd = data_socket;

    char reply[] = "200 Command okay.\r\n";

    if (!error_handling((int)tcp_send(client->fd, reply,
        strlen(reply)), "port : tcp_send"))
        return 1;

    return 0;
}
