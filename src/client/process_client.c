/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** process_client
*/

#include "ftp.h"

static void execute_command(client_t *client, char *arg)
{
    char *cmd = strtok_r(arg, " \r\n", &arg);
    char *param = strtok_r(NULL, "\r\n", &arg);
    size_t i = 0;
    int ret = 0;

    DEBUG_PRINT("\033[0;32m[DEBUG]\033[0m cmd: %s, param: %s\n", cmd, param);
    for (i = 0; commands[i].name; i++)
        if (strcmp(commands[i].name, cmd) == 0) {
            ret = commands[i].func(client, param);
            break;
        }
    if ((!commands[i].name || ret == 1) && client->fd != -1) {
        DEBUG_PRINT("\033[0;31m[DEBUG]\033[0m Unknown command: %s\n", cmd);
        dprintf(client->fd, "500 Unknown command.\r\n");
    }
}

static client_t *get_client_by_fd(struct client_head *head, int fd)
{
    client_t *client = NULL;

    TAILQ_FOREACH(client, head, entries) {
        if (client->fd == fd)
            return client;
    }
    return NULL;
}

int process_client(int client_fd, struct client_head *head)
{
    char buf[BUF_SIZE];
    int nbytes;
    client_t *client;

    nbytes = read(client_fd, buf, sizeof buf);
    if (nbytes <= 0) {
        if (nbytes == 0)
            printf("Connection closed by client %d\n", client_fd);
        else
            perror("recv");
        close(client_fd);
        return -1;
    } else {
        client = get_client_by_fd(head, client_fd);
        buf[nbytes] = '\0';
        DEBUG_PRINT("\033[0;32m[DEBUG]\033[0m Received: %s\n", buf);
        execute_command(client, buf);
    }
    return 0;
}
