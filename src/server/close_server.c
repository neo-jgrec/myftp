/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** close_server
*/

#include "ftp.h"

int close_server(int server_fd, struct client_head *head)
{
    client_t *np = NULL;

    while (!TAILQ_EMPTY(head)) {
        np = TAILQ_FIRST(head);
        TAILQ_REMOVE(head, np, entries);
        close(np->fd);
        free(np);
    }
    close(server_fd);
    return (EXIT_SUCCESS);
}
