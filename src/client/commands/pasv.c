/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pasv
*/

#include "ftp.h"
#include <stdbool.h>

static int set_data_socket(client_t *client)
{
    struct sockaddr_in data_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int data_socket = tcp_listen(0, 1);

    if (ERROR_HANDLING(data_socket, "pasv: tcp_listen failed") == -1)
        return 1;
    if (ERROR_HANDLING(getsockname(data_socket, &data_addr, &addrlen),
        "pasv: getsockname failed") == -1) {
        close(data_socket);
        return 1;
    }
    client->data_fd = data_socket;
    client->data_port = ntohs(data_addr.sin_port);
    if (!ERROR_HANDLING((char *)inet_ntop(AF_INET, &data_addr.sin_addr,
        client->data_ip, sizeof(client->data_ip)), "pasv: inet_ntop failed")) {
        close(data_socket);
        return 1;
    }
    return 0;
}

static int send_ip_and_port(client_t *client)
{
    char ip_and_port[255];
    struct sockaddr_in data_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    getsockname(client->data_fd, (struct sockaddr *)&data_addr, &addrlen);
    snprintf(ip_and_port, 255, "(%d,%d,%d,%d,%d,%d)",
            (unsigned char)data_addr.sin_addr.s_addr,
            (unsigned char)(data_addr.sin_addr.s_addr >> 8),
            (unsigned char)(data_addr.sin_addr.s_addr >> 16),
            (unsigned char)(data_addr.sin_addr.s_addr >> 24),
            client->data_port / 256, client->data_port % 256);
    if (tcp_send(client->fd, "227 Entering Passive Mode ", 26) == -1
        || tcp_send(client->fd, ip_and_port, strlen(ip_and_port)) == -1
        || tcp_send(client->fd, ".\r\n", 3) == -1)
        return 1;
    return 0;
}

int pasv(client_t *client, UNUSED char *arg)
{
    int data_socket = set_data_socket(client);

    if (data_socket != 0) {
        close(data_socket);
        return 1;
    }
    if (send_ip_and_port(client))
        return 1;
    client->transfer = PASSIVE_TRANSFER;
    return 0;
}
