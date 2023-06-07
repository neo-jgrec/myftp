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

    for (i = 0; commands[i].name; i++)
        if (strcmp(commands[i].name, cmd) == 0) {
            commands[i].func(client, param);
            break;
        }
    if (!commands[i].name)
        dprintf(client->fd, "xxx Error (RFC compliant)\r\n");
}

void process_client(client_t *client)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    dprintf(client->fd, "220 Service ready for new user.\r\n");

    while ((read = getline(&line, &len, fdopen(client->fd, "r"))) != -1) {
        printf("Client %d sent: %s", client->fd, line);
        execute_command(client, line);
    }

    printf("Client disconnected : %d\n", client->fd);
    free(line);
    close(client->fd);
    exit(EXIT_SUCCESS);
}
