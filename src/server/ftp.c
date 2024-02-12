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

static int listenner_loop(int server_fd, struct client_head *head,
    client_t *np, char *path)
{
    while (1) {
        if (setjmp(jump_buffer) == 1)
            break;
        np = calloc(1, sizeof(client_t));
        np->fd = tcp_accept(server_fd);
        if (!ERROR_HANDLING(np->fd, "Failed to accept client connection"))
            return (EXIT_FAILURE);
        np->username = strdup("Anonymous");
        np->cwd = strdup(path);
        TAILQ_INSERT_TAIL(head, np, entries);
        new_client(np);
    }
    return (EXIT_SUCCESS);
}

int ftp(int port, char *path)
{
    client_t *np = NULL;
    struct client_head head;
    int ret = EXIT_SUCCESS;
    int server_fd = 0;

    TAILQ_INIT(&head);
    if (!ERROR_HANDLING(path, "Invalid path")
        || !ERROR_HANDLING(port, "Invalid port")
        || !ERROR_HANDLING(chdir(path), "Invalid path"))
        return (EXIT_FAILURE);
    server_fd = tcp_listen(port, SOMAXCONN);
    if (!ERROR_HANDLING(server_fd, "Failed to create server socket"))
        return (EXIT_FAILURE);
    printf("Server listening on port %d\n", port);
    DEBUG_PRINT("\033[0;32m[DEBUG]\033[0m Server path: %s\n", path);
    signal(SIGINT, sigint_handler);
    ret = listenner_loop(server_fd, &head, np, path);
    close_server(server_fd, &head);
    printf("Server closed\n");
    return (ret);
}
