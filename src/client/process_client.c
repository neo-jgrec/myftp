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

    DEBUG_PRINT("\033[0;32m[DEBUG]\033[0m cmd: %s, param: %s\n", cmd, param);
    for (i = 0; commands[i].name; i++) {
        if (commands[i].need_login && !client->logged_in) {
            DEBUG_PRINT("\033[0;31m[DEBUG]\033[0m need login: %s\n", cmd);
            dprintf(client->fd, "530 Not logged in.\r\n");
            break;
        }
        if ((cmd && strcmp(commands[i].name, cmd) == 0)) {
            commands[i].func(client, param);
            break;
        }
    }
    if ((!commands[i].name) && client->fd != -1) {
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

static char *read_till_crlf(int fd)
{
    static char buf[BUF_SIZE];
    int nbytes = 0;
    char c = '\0';

    memset(buf, 0, BUF_SIZE);
    for (int i = 0; i < BUF_SIZE - 1; i++) {
        nbytes = read(fd, &c, 1);
        if (nbytes <= 0)
            return NULL;
        if (c == '\n' && i > 0 && buf[i - 1] == '\r') {
            buf[i - 1] = '\0';
            return buf;
        }
        buf[i] = c;
    }
    return NULL;
}

int process_client(int client_fd, struct client_head *head)
{
    char *buf = NULL;
    client_t *client = NULL;

    client = get_client_by_fd(head, client_fd);
    if (!client) {
        DEBUG_PRINT("\033[0;31m[DEBUG]\033[0m Client not found\n", "");
        return -1;
    }
    buf = read_till_crlf(client_fd);
    if (!buf) {
        DEBUG_PRINT("\033[0;31m[DEBUG]\033[0m read_till_crlf failed\n", "");
        return -1;
    }
    DEBUG_PRINT("\033[0;32m[DEBUG]\033[0m Received: %s\n", buf);
    execute_command(client, buf);
    return 0;
}
