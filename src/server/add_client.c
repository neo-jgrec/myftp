/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** add_client
*/

#include "ftp.h"

int new_client(client_t *client)
{
    pid_t pid = fork();
    char buffer[256] = {0};

    sprintf(buffer, "Failed to fork process for client %d", client->fd);

    if (!error_handling(pid, buffer))
        return (EXIT_FAILURE);
    else if (pid == 0) {
        printf("Client connected : %d\n", client->fd);
        process_client(client);
        exit(EXIT_SUCCESS);
    } else
        client->pid = pid;
    return (EXIT_SUCCESS);
}
