/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** connect
*/

#include "tcp_lib.h"

int tcp_connect(const char *server_ip, int server_port)
{
    #if defined(DEBUG)
    printf("\033[0;32m[LIBTCP DEBUG]\033[0m [CONNECT] [IP: %s] [PORT: %d]\n",
        server_ip, server_port);
    #endif
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    if (handle_error(client_socket, "socket error") == -1)
        return -1;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &(server_addr.sin_addr));
    if (handle_error(connect(client_socket, (struct sockaddr*)&server_addr,
        sizeof(server_addr)), "connect error") == -1)
        return -1;
    return client_socket;
}
