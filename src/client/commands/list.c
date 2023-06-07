/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** list
*/

#include "ftp.h"
#include <sys/wait.h>

int list(client_t *client, UNUSED char *arg)
{
    int pid = 0;
    int status = 0;

    dprintf(client->fd, "150 File status okay; about to open data connection.\r\n");
    pid = fork();
    if (pid == 0) {
        dup2(client->data_fd, STDOUT_FILENO);
        dup2(client->data_fd, STDERR_FILENO);
        execl("/bin/ls", "ls", "-l", NULL);
    } else {
        waitpid(pid, &status, 0);
        close(client->data_fd);
        dprintf(client->fd, "226 Closing data connection.\r\n");
    }
    return (EXIT_SUCCESS);
}
