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

int port(client_t *client, char *arg)
{
    struct ip_data_struct ip_data;

    sscanf(arg, "%u,%u,%u,%u,%u,%u",
        &ip_data.h1, &ip_data.h2, &ip_data.h3, &ip_data.h4,
        &ip_data.p1, &ip_data.p2);
    ip_data.port = (ip_data.p1 * 256) + ip_data.p2;
    client->data_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->data_fd == -1) {
        dprintf(client->fd, "425 Can't open data connection.\r\n");
        return 1;
    }
    client->data_addr.sin_family = AF_INET;
    client->data_addr.sin_port = htons(ip_data.port);
    client->data_addr.sin_addr.s_addr = htonl((ip_data.h1 << 24)
        | (ip_data.h2 << 16) | (ip_data.h3 << 8) | ip_data.h4);
    printf("IP: %s\n", inet_ntoa(client->data_addr.sin_addr));
    printf("PORT: %d\n", ntohs(client->data_addr.sin_port));
    dprintf(client->fd, "%s", reply);
    client->transfer = ACTIVE_TRANSFER;
    return 0;
}
