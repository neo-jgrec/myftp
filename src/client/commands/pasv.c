/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pasv
*/

#include "ftp.h"

int pasv(client_t *client, UNUSED char *arg)
{
    int data_server_socket = tcp_listen(0, SOMAXCONN);
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);

    if (!error_handling(data_server_socket, "pasv : tcp_listen")
        || !error_handling(getsockname(data_server_socket,
        (struct sockaddr *)&sin, &len), strerror(errno)))
        return 1;
    unsigned short data_port = ntohs(sin.sin_port);
    unsigned short p1 = data_port >> 8;
    unsigned short p2 = data_port & 255;
    client->transfer = PASSIVE_TRANSFER;
    client->data_port = data_port;
    client->data_fd = data_server_socket;
    char reply[4096];
    snprintf(reply, 4096, "227 Entering Passive Mode (%s,%u,%u)\r\n", client->ip, p1, p2);
    if (!error_handling((int)tcp_send(client->fd, reply,
        strlen(reply)), "pasv : tcp_send"))
        return 1;
    return 0;
}
