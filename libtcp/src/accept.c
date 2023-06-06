/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** accept
*/

#include "tcp_lib.h"

int tcp_accept(int server_socket)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(server_socket,
        (struct sockaddr*)&client_addr, &client_addr_size);

    if (handle_error(client_socket, "accept error") == -1)
        return -1;
    return client_socket;
}
