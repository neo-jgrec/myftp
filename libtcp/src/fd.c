/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** fd
*/

#include "tcp_lib.h"

void tcp_fd_set_init(fd_set *set, int sockfd)
{
    FD_ZERO(set);
    FD_SET(sockfd, set);
}

void tcp_fd_set_add(int sockfd, fd_set *set)
{
    FD_SET(sockfd, set);
}

void tcp_fd_set_clear(int sockfd, fd_set *set)
{
    FD_CLR(sockfd, set);
}
