/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** syst
*/

#include "ftp.h"

int syst(client_t *client, char *arg)
{
    (void)arg;
    return (dprintf(client->fd, "215 UNIX Type: L8\r\n"), 0);
}
