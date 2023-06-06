/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** server
*/

#include "tcp_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "example.h"

/**
 * @brief Array of client sockets
 *
 * Used to manage the connections with all connected clients
 */
int client_sockets[MAXCLIENTS];

/**
 * @brief Array of client sockets
 *
 * Used to manage the connections with all connected clients
 */
int client_sockets[MAXCLIENTS];

/**
 * @brief Initializes the array of client sockets
 *
 * Sets all values in the array to 0, representing no clients connected
 */
void init_client_sockets(void)
{
    for (int i = 0; i < MAXCLIENTS; i++) {
        client_sockets[i] = 0;
    }
}

/**
 * @brief Adds a client socket to the array
 *
 * Inserts the socket descriptor into the first available position in the array
 *
 * @param client_socket The socket descriptor for the new client connection
 */
void add_client(int client_socket)
{
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (client_sockets[i] == 0) {
            client_sockets[i] = client_socket;
            break;
        }
    }
}

/**
 * @brief Removes a client socket from the array
 *
 * Finds the socket descriptor in the array and sets its value to 0
 *
 * @param client_socket The socket descriptor for the client connection to be removed
 */
void remove_client(int client_socket)
{
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (client_sockets[i] == client_socket) {
            client_sockets[i] = 0;
            break;
        }
    }
}

/**
 * @brief Broadcasts a message to all clients
 *
 * Sends a message to all clients except the client from which the message originated
 *
 * @param from_socket The socket descriptor for the client that sent the message
 * @param message The message to be broadcast
 * @param length The length of the message to be broadcast
 */
void broadcast_message(int from_socket, char *message, size_t length)
{
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (client_sockets[i] > 0 && client_sockets[i] != from_socket) {
            tcp_send(client_sockets[i], message, length);
        }
    }
}

/**
 * @brief Handles SIGINT signals
 *
 * Closes all client sockets and exits the program when a SIGINT signal is received
 *
 * @param signum The signal number
 */
void sigint_handler(int signum)
{
    (void)signum;
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (client_sockets[i] > 0) {
            close(client_sockets[i]);
        }
    }
    exit(0);
}

/**
 * @brief Main function
 *
 * This function sets up a TCP server, manages client connections,
 * and handles sending and receiving of messages.
 *
 * Usage: ./server <port>
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return int Exit status
 */
int main(int argc, char **argv)
{
    // Register the SIGINT handler
    signal(SIGINT, sigint_handler);

    // Validate command line arguments
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    // Set up the server
    int server_socket = tcp_listen(atoi(argv[1]), 10);
    if (server_socket < 0) {
        perror("Failed to set up server");
        return 1;
    }

    // Initialize the array of client sockets
    init_client_sockets();

    // Initialize the fd_set for select()
    fd_set read_fds;
    tcp_fd_set_init(server_socket, &read_fds);

    // Buffer to hold incoming messages
    char buffer[MAXMSG];

    // Keep track of the highest file descriptor number for select()
    int max_fd = server_socket;

    // Main server loop
    while (1) {
        // Create a temporary fd_set for select()
        fd_set temp_set = read_fds;

        // Use select() to wait for ready sockets
        int status = tcp_select(max_fd, &temp_set);
        if (status < 0) {
            // Error occurred, reinitialize read_fds and continue the loop
            tcp_fd_set_clear(server_socket, &read_fds);
            tcp_fd_set_init(server_socket, &read_fds);
            continue;
        }

        // Check if there are any new connections on the server socket
        if (FD_ISSET(server_socket, &temp_set)) {
            // Accept the new client connection
            int client_socket = tcp_accept(server_socket);
            if (client_socket < 0) {
                // Error occurred, continue the loop
                continue;
            }
            // Successfully connected to a new client, add to the client sockets array
            add_client(client_socket);

            // Add the new client socket to read_fds
            tcp_fd_set_add(client_socket, &read_fds);

            // Update max_fd if necessary
            if (client_socket > max_fd) {
                max_fd = client_socket;
            }
        }

        // Loop through all clients and check if there are any incoming messages
        for(int i = 0; i < MAXCLIENTS; i++) {
            int sock = client_sockets[i];
            if (FD_ISSET(sock, &temp_set)) {
                // Receive the message from the client
                ssize_t bytes_received = tcp_recv(sock, buffer, sizeof(buffer) - 1);

                if (bytes_received <= 0) {
                    // Client has disconnected, close the socket, remove from client sockets array and read_fds
                    close(sock);
                    remove_client(sock);
                    tcp_fd_set_clear(sock, &read_fds);
                    printf("Client disconnected\n");
                } else {
                    // Message received from the client, broadcast to all other clients
                    buffer[bytes_received] = '\0';
                    broadcast_message(sock, buffer, bytes_received);
                }
            }
        }
    }

    // Close the server socket before exiting
    close(server_socket);

    return 0;
}
