/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** ftp
*/

#include "ftp.h"
#include <sys/wait.h>


int ftp(int port, char *path)
{
    client_t *np = NULL;
    struct ClientHead head;

    TAILQ_INIT(&head);
    if (!error_handling(path, "Invalid path")
        || !error_handling(port, "Invalid port"))
        return (EXIT_FAILURE);
    int server_fd = tcp_listen(port, SOMAXCONN);
    if (!error_handling(server_fd, "Failed to create server socket"))
        return (EXIT_FAILURE);
    printf("Server listening on port %d\n", port);
    while (1) {
        np = calloc(1, sizeof(client_t));
        np->fd = tcp_accept(server_fd);
        if (!error_handling(np->fd, "Failed to accept client connection"))
            return (EXIT_FAILURE);
        TAILQ_INSERT_TAIL(&head, np, entries);
        new_client(np);
    }
    return (EXIT_SUCCESS);
}
