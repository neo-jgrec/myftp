/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** ftp
*/

#ifndef FTP_H_
    #define FTP_H_

    #define EXIT_FAILURE 84
    #define EXIT_SUCCESS 0

    #define UNUSED __attribute__((unused))

    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>

    #include "tcp_lib.h"
    #include "error_handling.h"
    #include "client.h"

/**
 * @brief Starts the FTP server.
 *
 * This function starts the FTP server.
 * It creates a socket, binds it to the specified port, and listens for
 * incoming connections.
 *
 * @param port The port to listen on.
 * The port must be between 0 and 65535.
 * @param path The path to the home directory for the Anonymous user.
 * @return int EXIT_SUCCESS if the server exited successfully, EXIT_FAILURE
 * otherwise.
 *
 * @note When a client is connected, a new process is created to handle it.
 * This process is terminated when the client disconnects.
 */
int ftp(int port, char *path);

/**
 * @brief Processes a client.
 *
 * This function processes a client.
 * It reads the client's input, and calls the appropriate function.
 *
 * @param client The client to process.
 */
void process_client(client_t *client);

/**
 * @brief Creates a new client.
 *
 * This function creates a new client.
 * It forks the current process, and calls process_client() in the child
 * process.
 *
 * @param client The client to create.
 * @return int EXIT_SUCCESS if the client was created successfully,
 * EXIT_FAILURE otherwise.
 */
int new_client(client_t *client);

#endif /* !FTP_H_ */
