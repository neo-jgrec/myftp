/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** get_port
*/

#include "ftp.h"

size_t get_port(char *arg)
{
    char *token = NULL;
    char *saveptr = NULL;
    unsigned int port = 0;
    int i = 0;

    token = strtok_r(arg, ",", &saveptr);
    while (token != NULL) {
        if (i == 4)
            port += atoi(token);
        else
            port += atoi(token) << (8 * (3 - i));
        token = strtok_r(NULL, ",", &saveptr);
        i++;
    }
    return (port);
}
