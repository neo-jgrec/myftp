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


int main(int argc, char **argv)
{
    int port = 0;
    char *address = NULL;
    int isHelpRequested = argc > 1 && (!strcmp(argv[1], "--help")
        || !strcmp(argv[1], "-h"));

    if (argc != 3 || isHelpRequested) {
        dprintf(isHelpRequested ? STDOUT_FILENO : STDERR_FILENO, "%s", usage);
        return isHelpRequested ? 0 : 84;
    }
    port = atoi(argv[1]);
    address = argv[2];
    return ftp(port, address);
}
