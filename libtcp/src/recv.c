/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** recv
*/

#include "tcp_lib.h"

ssize_t tcp_recv(int socket, void *buffer, size_t length)
{
    #if defined(DEBUG)
    printf("\033[0;32m[LIBTCP DEBUG]\033[0m [RECV] [SOCKET: %d] [BUFFER: %p] "
        "[LENGTH: %ld]\n", socket, buffer, length);
    #endif
    fd_set read_fds;
    struct timeval timeout = {0, 0};
    int result;
    FD_ZERO(&read_fds);
    FD_SET(socket, &read_fds);

    result = select(socket + 1, &read_fds, NULL, NULL, &timeout);
    if (handle_error(result, "select recv") == -1) return -1;
    if (result > 0 && FD_ISSET(socket, &read_fds)) {
        return read(socket, buffer, length);
    } else {
        dprintf(2, "Timeout, no data received\n");
        return 0;
    }
    return -1;
}
