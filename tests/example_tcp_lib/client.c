/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** client
*/

#include "tcp_lib.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "example.h"
#include <signal.h>
#include <sys/select.h>

int client_socket;

/**
 * @brief Handle SIGINT signal
 *
 * Close the client socket and exit the program
 * So we do not have a zombie program and to wait for the timeout
 *
 * @param signum
 */
void sigint_handler(int signum)
{
    (void)signum;
    close(client_socket);
    exit(0);
}

/**
 * @brief Main function
 *
 * This function sets up a TCP client, connects to a specified server, and handles
 * the sending and receiving of messages.
 *
 * Usage: ./client <server IP> <port>
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
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        return 1;
    }

    // Extract server IP and port from command line arguments
    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    // Connect to the server
    client_socket = tcp_connect(server_ip, server_port);
    if (client_socket < 0) {
        perror("Failed to connect to server");
        return 1;
    }

    // Initialize the fd_set for select()
    fd_set read_fds;
    tcp_fd_set_init(client_socket, &read_fds);
    tcp_fd_set_add(STDIN_FILENO, &read_fds);

    // Buffers to hold sent and received messages
    char send_buffer[MAXMSG];
    char recv_buffer[MAXMSG];

    // Main client loop
    while (1) {
        // Create a temporary fd_set for select()
        fd_set temp_set = read_fds;

        // Use select() to wait for ready sockets
        if (tcp_select(client_socket+1, &temp_set) < 0)
            break;

        // Check for incoming messages from the server
        if (FD_ISSET(client_socket, &temp_set)) {
            // Receive the message from the server
            ssize_t bytes_received = tcp_recv(client_socket, recv_buffer, sizeof(recv_buffer) - 1);
            if (bytes_received <= 0)
                break;

            // Null-terminate and print the received message
            recv_buffer[bytes_received] = '\0';
            printf("Received: %s\n", recv_buffer);
        }

        // Check for input from stdin
        if (FD_ISSET(STDIN_FILENO, &temp_set)) {
            // Read input from stdin
            fgets(send_buffer, sizeof(send_buffer), stdin);

            // Send the input to the server
            ssize_t bytes_sent = tcp_send(client_socket, send_buffer, strlen(send_buffer));
            if (bytes_sent < 0) {
                perror("Failed to send data to server");
                break;
            }
        }
    }

    // Close the client socket before exiting
    close(client_socket);

    return 0;
}
