/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** listen
*/

#include "tcp_lib.h"

int tcp_listen(int server_port, int backlog)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    struct sockaddr_in server_addr;

    if (handle_error(server_socket, "socket error") == -1)
        return -1;
    if (handle_error(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
        &optval, sizeof(optval)), "setsockopt error") == -1)
        return -1;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (handle_error(bind(server_socket, (struct sockaddr*)&server_addr,
        sizeof(server_addr)), "bind error") == -1)
        return -1;
    if (handle_error(listen(server_socket, backlog), "listen error") == -1)
        return -1;
    return server_socket;
}
