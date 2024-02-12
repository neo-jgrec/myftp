/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** port
*/

#include "ftp.h"

const char reply[] = "200 Command okay.\r\n";

struct ip_data_struct {
    unsigned int h1;
    unsigned int h2;
    unsigned int h3;
    unsigned int h4;
    unsigned int p1;
    unsigned int p2;
    unsigned short port;
};

static struct ip_data_struct create_ip_data_struct(void)
{
    struct ip_data_struct ip_data;

    ip_data.h1 = 0;
    ip_data.h2 = 0;
    ip_data.h3 = 0;
    ip_data.h4 = 0;
    ip_data.p1 = 0;
    ip_data.p2 = 0;
    ip_data.port = 0;
    return ip_data;
}

int port(client_t *client, char *arg)
{
    int data_socket = 0;
    char ip[16];
    struct ip_data_struct ip_data = create_ip_data_struct();

    sscanf(arg, "%u,%u,%u,%u,%u,%u", &ip_data.h1, &ip_data.h2,
        &ip_data.h3, &ip_data.h4, &ip_data.p1, &ip_data.p2);
    ip_data.port = ip_data.p1 * 256 + ip_data.p2;
    snprintf(ip, 16, "%u.%u.%u.%u", ip_data.h1,
        ip_data.h2, ip_data.h3, ip_data.h4);
    data_socket = tcp_connect(ip, ip_data.port);
    if (!ERROR_HANDLING(data_socket, "port : tcp_connect"))
        return 1;
    client->transfer = ACTIVE_TRANSFER;
    client->data_port = ip_data.port;
    client->data_fd = data_socket;
    if (!ERROR_HANDLING((int)tcp_send(client->fd, reply,
        strlen(reply)), "port : tcp_send"))
        return 1;
    return 0;
}
