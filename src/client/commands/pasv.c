/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pasv
*/

#include "ftp.h"

static int set_data_socket(client_t *client)
{
    int data_socket;
    struct sockaddr_in data_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    memset(&data_addr, 0, sizeof(data_addr));
    data_addr.sin_family = AF_INET;
    data_addr.sin_addr.s_addr = INADDR_ANY;
    data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!error_handling(data_socket, "pasv : socket")) return 1;
    if (bind(data_socket, (struct sockaddr*)&data_addr, sizeof(data_addr)) < 0
        || listen(data_socket, 1) == -1)
        return data_socket;
    getsockname(data_socket, (struct sockaddr *)&data_addr, &addrlen);
    client->data_fd = data_socket;
    client->data_port = ntohs(data_addr.sin_port);
    getsockname(client->fd, (struct sockaddr *)&data_addr, &addrlen);
    if (!inet_ntop(AF_INET, &data_addr.sin_addr, client->data_ip,
        sizeof(client->data_ip)))
        return data_socket;
    return 0;
}

static int send_ip_and_port(client_t *client)
{
    char ip_and_port[255];
    struct sockaddr_in data_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    getsockname(client->fd, (struct sockaddr *)&data_addr, &addrlen);
    snprintf(ip_and_port, 255, "(%d,%d,%d,%d,%d,%d)",
            (unsigned char)data_addr.sin_addr.s_addr,
            (unsigned char)(data_addr.sin_addr.s_addr >> 8),
            (unsigned char)(data_addr.sin_addr.s_addr >> 16),
            (unsigned char)(data_addr.sin_addr.s_addr >> 24),
            (client->data_port >> 8) & 0xFF,
            client->data_port & 0xFF);

    tcp_send(client->fd, "227 Entering Passive Mode ", 25);
    tcp_send(client->fd, ip_and_port, strlen(ip_and_port));
    tcp_send(client->fd, "\r\n", 2);
    return 0;
}

int pasv(client_t *client, UNUSED char *arg)
{
    int data_socket;

    if ((data_socket = set_data_socket(client)) != 0) {
        close(data_socket);
        return 1;
    }
    if (send_ip_and_port(client))
        return 1;
    client->transfer = PASSIVE_TRANSFER;
    return 0;
}
