/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** main
*/

#include "ftp.h"

static const char *usage =
    "USAGE: ./myftp port path\n"
    "\tport\tis the port number on which the server socket listens\n"
    "\tpath\tis the path to the home directory for the Anonymous user\n";

int main(int ac, char **av)
{
    if (ac != 3 || (!strcmp(av[1], "--help") || !strcmp(av[1], "-h"))) {
        dprintf((ac != 3 ? 2 : 1), "%s", usage);
        return ((ac != 3) || (!strcmp(av[1], "--help") || !strcmp(av[1], "-h"))
            ? EXIT_FAILURE : EXIT_SUCCESS);
    }
    return (ftp(atoi(av[1]), av[2]));
}
