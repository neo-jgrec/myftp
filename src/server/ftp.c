/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** ftp
*/

#include "ftp.h"
#include <sys/signal.h>
#include <setjmp.h>

jmp_buf jump_buffer;

static void sigint_handler(int signo)
{
    (void)signo;
    longjmp(jump_buffer, 1);
}

static int listenner_loop(int server_fd, struct client_head head, client_t *np)
{
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

int ftp(int port, char *path)
{
    client_t *np = NULL;
    struct client_head head;
    int ret = EXIT_SUCCESS;

    TAILQ_INIT(&head);
    if (!error_handling(path, "Invalid path")
        || !error_handling(port, "Invalid port")
        || !error_handling(chdir(path), strerror(errno)))
        return (EXIT_FAILURE);
    int server_fd = tcp_listen(port, SOMAXCONN);
    if (!error_handling(server_fd, "Failed to create server socket"))
        return (EXIT_FAILURE);
    printf("Server listening on port %d\n", port);
    signal(SIGINT, sigint_handler);
    ret = (setjmp(jump_buffer) == 0)
        ? listenner_loop(server_fd, head, np)
        : close_server(server_fd, &head);
    printf("Server closed\n");
    return (ret);
}
