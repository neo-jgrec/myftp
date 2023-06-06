/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** select
*/

#include "tcp_lib.h"

int tcp_select(int maxfd, fd_set *set)
{
    int status = select(maxfd + 1, set, NULL, NULL, NULL);

    if (handle_error(status, "select") == -1)
        return -1;
    return status;
}
