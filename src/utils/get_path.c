/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** get_path
*/

#include "ftp.h"

char *get_path(char *cwd, char *arg)
{
    char *path = NULL;

    if (arg[0] == '/')
        path = strdup(arg);
    else {
        path = malloc(sizeof(char) * (strlen(cwd) + strlen(arg) + 2));
        asprintf(&path, "%s/%s", cwd, arg);
    }
    return (path);
}
