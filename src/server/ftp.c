/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** ftp
*/

#include "ftp.h"
#include <sys/signal.h>
#include <setjmp.h>

static jmp_buf jump_buffer;

static void sigint_handler(int signo)
{
    (void)signo;
    longjmp(jump_buffer, 1);
}

static int handle_new_connection(server_data_t *data,
    fd_set *master_fds, int *max_fd)
{
    int new_fd = accept(data->fd, NULL, NULL);

    if (new_fd == -1) {
        perror("accept");
    } else {
        FD_SET(new_fd, master_fds);
        *max_fd = new_fd > *max_fd ? new_fd : *max_fd;
        new_client(new_fd, data->head, data->path);
    }
    return (EXIT_SUCCESS);
}

static int process_client_connection(server_data_t *data, int client_fd,
    fd_set *master_fds)
{
    if (process_client(client_fd, data->head, data->path) == -1) {
        close(client_fd);
        FD_CLR(client_fd, master_fds);
    }
    return (EXIT_SUCCESS);
}

static int process_connections(server_data_t *data,
    fd_set *master_fds, int *max_fd)
{
    fd_set read_fds;

    read_fds = *master_fds;
    if (!ERROR_HANDLING(tcp_select(*max_fd, &read_fds), "select"))
        return (EXIT_FAILURE);
    for (int i = 0; i <= *max_fd; i++) {
        if (!FD_ISSET(i, &read_fds))
            continue;
        if (i == data->fd)
            handle_new_connection(data, master_fds, max_fd);
        else
            process_client_connection(data, i, master_fds);
    }
    return 0;
}

static int listener_loop(int server_fd,
    struct client_head *head, const char *path)
{
    fd_set master_fds;
    int max_fd = server_fd;

    tcp_fd_set_init(&master_fds, server_fd);
    while (1) {
        if (process_connections(&(server_data_t){server_fd, head, path},
            &master_fds, &max_fd) == -1)
            return 84;
    }
    return (EXIT_SUCCESS);
}

int ftp(int port, char *path_raw)
{
    struct client_head head;
    int r = EXIT_SUCCESS;
    int server_fd = 0;
    const char *path = realpath(path_raw, NULL);

    TAILQ_INIT(&head);
    if (!ERROR_HANDLING(path_raw, "Invalid path")
        || !ERROR_HANDLING(port, "Invalid port")
        || !ERROR_HANDLING(chdir(path), "Invalid path"))
        return 84;
    server_fd = tcp_listen(port, SOMAXCONN);
    if (!ERROR_HANDLING(server_fd, "Failed to create server socket"))
        return 84;
    printf("Server listening on port %d\n", port);
    DEBUG_PRINT("\033[0;32m[DEBUG]\033[0m Server path: %s\n", path);
    signal(SIGINT, sigint_handler);
    r = setjmp(jump_buffer) == 0 ? listener_loop(server_fd, &head, path) : r;
    close_server(server_fd, &head);
    printf("Server closed\n");
    return (r);
}
