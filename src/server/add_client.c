/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** add_client
*/

#include "ftp.h"

int new_client(int fd, struct client_head *head, const char *path)
{
    client_t *np = calloc(1, sizeof(client_t));

    if (!np) {
        perror("Failed to allocate memory for new client");
        close(fd);
        return EXIT_FAILURE;
    }
    np->transfer = NO_TRANSFER;
    np->data_fd = -1;
    np->data_port = -1;
    np->ip = get_ip();
    np->username = NULL;
    np->password = NULL;
    np->fd = fd;
    np->logged_in = false;
    np->cwd = strdup(path);
    TAILQ_INSERT_TAIL(head, np, entries);
    dprintf(fd, "220 Service ready for new user.\r\n");
    return EXIT_SUCCESS;
}
