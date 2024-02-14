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
    #ifndef _GNU_SOURCE
        #define _GNU_SOURCE
    #endif

    #ifdef DEBUG
        #ifndef fprintf
            #include <stdio.h>
        #endif
        #define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
    #else
        #define DEBUG_PRINT(fmt, ...)
    #endif

    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <errno.h>

    #include "tcp_lib.h"
    #include "error_handling.h"
    #include "client.h"

    #define MAX_CLIENTS 1024
    #define BUF_SIZE 1024

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
int process_client(int client_fd, struct client_head *head);

/**
 * @brief Creates a new client.
 *
 * This function creates a new client.
 * process.
 *
 * @param client The client to create.
 * @return int EXIT_SUCCESS if the client was created successfully,
 * EXIT_FAILURE otherwise.
 */
int new_client(int fd, struct client_head *head);

/**
 * @brief Closes the server.
 *
 * This function closes the server.
 * It closes the server socket, and all the client sockets.
 *
 * @param server_fd The server socket.
 * @param head The head of the client list.
 *
 * @return int EXIT_SUCCESS if the server was closed successfully,
 */
int close_server(int server_fd, struct client_head *head);

/**
 * @brief Gets the server's IP address.
 *
 * This function gets the server's IP address.
 * It creates a socket, connects it to a remote host, and gets the IP address
 * of the socket.
 *
 * @return char * The server's IP address.
 */
char *get_ip(void);

/**
 * @brief Gets the path of the provided argument.
 *
 * This function gets the path of the provided argument.
 * It concatenates the current working directory and the provided argument.
 *
 * @param cwd The current working directory.
 * @param arg The argument.
 *
 * @return char * The path of the provided argument.
 */
char *get_path(char *cwd, char *arg);

/**
 * @brief Gets the port from the provided argument.
 *
 * This function gets the port from the provided argument.
 * It splits the argument into 6 parts, and calculates the port from these
 * parts.
 *
 * @param arg The argument.
 *
 * @return unsigned int The port.
 */
size_t get_port(char *arg);

#endif /* !FTP_H_ */
