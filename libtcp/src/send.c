/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** send
*/

#include "tcp_lib.h"

ssize_t tcp_send(int socket, const void *buffer, size_t length)
{
    #if defined(DEBUG)
    printf("\033[0;32m[LIBTCP DEBUG]\033[0m [SEND] [SOCKET: %d] [BUFFER: %p] "
        "[LENGTH: %ld]\n", socket, buffer, length);
    #endif
    return write(socket, buffer, length);
}
