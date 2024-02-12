/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** syst
*/

#include "ftp.h"

int syst(client_t *client, UNUSED char *arg)
{
    #if defined(_WIN32)
        return (dprintf(client->fd, "215 Windows_NT Type: L8\r\n"), 0);
    #elif defined(__linux__)
        return (dprintf(client->fd, "215 UNIX Type: L8\r\n"), 0);
    #elif defined(__APPLE__) && defined(__MACH__)
        return (dprintf(client->fd, "215 MacOS Type: L8\r\n"), 0);
    #else
        return (dprintf(client->fd, "215 Unknown Type: L8\r\n"), 0);
    #endif
}
