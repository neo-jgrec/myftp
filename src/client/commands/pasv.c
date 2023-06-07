/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pasv
*/

#include "ftp.h"
#include <ifaddrs.h>

static char *get_ip(void)
{
    char *ip = NULL;
    struct ifaddrs *ifaddr = NULL;
    struct ifaddrs *ifa = NULL;

    if (getifaddrs(&ifaddr) == -1)
        return (NULL);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr
            && ifa->ifa_addr->sa_family == AF_INET
            && strcmp(ifa->ifa_name, "lo") != 0) {
            ip = inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr);
            break;
        }
    }
    freeifaddrs(ifaddr);
    return (ip);
}

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
