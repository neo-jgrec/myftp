/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** list
*/

#include "ftp.h"
#include <sys/types.h>
#include <sys/wait.h>

static const char *reply_start = "150 Here comes the directory listing.\r\n";
static const char *reply_complete = "226 Directory send OK.\r\n";

static int exec_ls(char *arg, int client_data_fd)
{
    char *command = NULL;
    FILE *pipe = NULL;
    char buffer[4096];

    asprintf(&command, "/bin/ls -l %s", (arg) ? arg : "");
    pipe = popen(command, "r");
    if (!ERROR_HANDLING(pipe, "popen listing"))
        return 1;
    while (!feof(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
            tcp_send(client_data_fd, buffer, strlen(buffer));
    }
    pclose(pipe);
    free(command);
    return 0;
}

static int fork_exec_ls(char *arg, int client_data_fd)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        exec_ls(arg, client_data_fd);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }
    return 0;
}

static int list_passive(client_t *client, char *arg)
{
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int client_data_fd;

    tcp_send(client->fd, reply_start, strlen(reply_start));
    client_data_fd = accept(client->data_fd, (struct sockaddr *)&client_addr,
        &addrlen);
    if (client_data_fd == -1)
        return 1;
    if (fork_exec_ls(arg, client_data_fd))
        return 1;
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client_data_fd);
    close(client->data_fd);
    client->data_fd = -1;
    return 0;
}

static int list_port(client_t *client, char *arg)
{
    tcp_send(client->fd, reply_start, strlen(reply_start));
    if (fork_exec_ls(arg, client->data_fd))
        return 1;
    tcp_send(client->fd, reply_complete, strlen(reply_complete));
    close(client->data_fd);
    client->data_fd = -1;
    return 0;
}

int list(client_t *client, char *arg)
{
    if (client->transfer == PASSIVE_TRANSFER) {
        return list_passive(client, arg);
    } else if (client->transfer == ACTIVE_TRANSFER) {
        return list_port(client, arg);
    } else {
        return 1;
    }
}
