/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** tcp_lib
*/

#ifndef TCP_LIB_H_
    #define TCP_LIB_H_

    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <errno.h>
    #include <execinfo.h>
    #include <stdlib.h>
    #include <sys/select.h>

/**
 * @brief Establishes a TCP connection to a specified server.
 *
 * This function creates a socket and attempts to establish a TCP connection to
 * a specified server.
 *
 * @param server_ip The IP address of the server to connect to.
 * This must be a string in dot-decimal notation, such as "192.168.0.1".
 * DNS names are not supported.
 * @param server_port The port number on the server to connect to.
 * This must be an integer in the range 0-65535.
 *
 * @return On successful connection, this function returns
 * the file descriptor of the client socket.
 * If an error occurs at any point, this function will return -1.
 *
 * @example
 * // Establish a connection to a server at 192.168.1.100 on port 8080
 * int sockfd = tcp_connect("192.168.1.100", 8080);
 * if (sockfd < 0) {
 *     perror("Failed to connect to server");
 *     exit(EXIT_FAILURE);
 * }
 * // Socket connected successfully, sockfd can be used to send/receive data
 */
int tcp_connect(const char *server_ip, int server_port);

/**
 * @brief Sets up a TCP server and starts listening for connections.
 *
 * This function creates a socket, binds it to a specified port,
 * and starts listening for incoming TCP connections.
 *
 * @param server_port The port number to bind the server socket to.
 * This must be an integer in the range 0-65535.
 * @param backlog The maximum length of the queue of pending connections.
 * We can use the SOMAXCONN macro to set this to the maximum value allowed
 *
 * @return On successful setup,
 * this function returns the file descriptor of the server
 * socket. If an error occurs at any point, this function will return -1.
 *
 * @example
 * // Set up a server listening on port 8080, with a backlog of 5
 * int sockfd = tcp_listen(8080, 5);
 * if (sockfd < 0) {
 *     perror("Failed to set up server");
 *     exit(EXIT_FAILURE);
 * }
 * // Server set up successfully,
 * // sockfd can be used to accept incoming connections
 */
int tcp_listen(int server_port, int backlog);

/**
 * @brief Accepts an incoming connection on a server socket.
 *
 * This function accepts an incoming connection
 * on a server socket and returns a new socket
 * file descriptor for the accepted connection.
 *
 * @param server_socket The file descriptor of the server socket.
 *
 * @return On successful acceptance,
 * this function returns the file descriptor of the client
 * socket. If an error occurs at any point, this function will return -1.
 *
 * @example
 * // Accept an incoming connection on the server socket
 * int new_sockfd = tcp_accept(sockfd);
 * if (new_sockfd < 0) {
 *     perror("Failed to accept incoming connection");
 *     exit(EXIT_FAILURE);
 * }
 * // Connection accepted successfully,
 * // new_sockfd can be used to send/receive data
 */
int tcp_accept(int server_socket);

/**
 * @brief Sends data over a TCP connection.
 *
 * This function writes the specified number
 * of bytes from a buffer to a specified socket.
 *
 * @param socket The file descriptor of the socket to write to.
 * @param buffer The buffer containing the data to write.
 * @param length The number of bytes to write.
 *
 * @return On successful write, this function returns
 * the number of bytes actually written.
 * If an error occurs, this function will return -1.
 *
 * @example
 * // Send a string to the client
 * char *msg = "Hello, client!";
 * ssize_t bytes_sent = tcp_send(new_sockfd, msg, strlen(msg));
 * if (bytes_sent < 0) {
 *     perror("Failed to send data to client");
 *     exit(EXIT_FAILURE);
 * }
 */
ssize_t tcp_send(int socket, const void *buffer, size_t length);

/**
 * @brief Receives data over a TCP connection.
 *
 * This function reads a specified number of bytes from a socket into a buffer.
 *
 * @param socket The file descriptor of the socket to read from.
 * @param buffer The buffer to read the data into.
 * @param length The maximum number of bytes to read.
 *
 * @return On successful read, this function returns
 * the number of bytes actually read.
 * If an error occurs, this function will return -1.
 *
 * @example
 * // Receive data from the client
 * char buffer[1024];
 * ssize_t bytes_received = tcp_recv(new_sockfd, buffer, sizeof(buffer) - 1);
 * if (bytes_received < 0) {
 *     perror("Failed to receive data from client");
 *     exit(EXIT_FAILURE);
 * }
 * // Null-terminate the received data to safely use it as a string
 * buffer[bytes_received] = '\0';
 */
ssize_t tcp_recv(int socket, void *buffer, size_t length);

/**
 * @brief Initialize a file descriptor set with a single socket descriptor.
 *
 * This function initializes the fd_set pointed to by "set", and adds the
 * socket descriptor "sockfd" to this set.
 *
 * @param sockfd The socket descriptor to be added to the set.
 * @param set Pointer to the fd_set to be initialized.
 *
 * @example
 * fd_set my_set;
 * tcp_fd_set_init(my_socket, &my_set);
 */
void tcp_fd_set_init(fd_set *set, int sockfd);

/**
 * @brief Add a socket descriptor to an existing file descriptor set.
 *
 * This function adds the socket descriptor "sockfd" to the fd_set pointed
 * to by "set".
 *
 * @param sockfd The socket descriptor to be added to the set.
 * @param set Pointer to the fd_set to which the descriptor is added.
 *
 * @example
 * tcp_fd_set_add(another_socket, &my_set);
 */
void tcp_fd_set_add(int sockfd, fd_set *set);

/**
 * @brief Perform a select operation on a file descriptor set.
 *
 * This function performs a select operation on the fd_set pointed to by "set".
 * It will block until a descriptor in the set is ready for reading.
 *
 * @param maxfd The highest-numbered file descriptor in the set + 1.
 * @param set Pointer to the fd_set on which to perform the select operation.
 *
 * @return -1 in case of error,
 *         otherwise the number of descriptors contained in
 *         the descriptor sets, which may be zero if the timeout expires before
 *         anything interesting happens.
 *
 * @example
 * int status = tcp_select(max_socket + 1, &my_set);
 * if (status < 0) {
 *     // handle error
 * }
 */
int tcp_select(int maxfd, fd_set *set);

/**
 * @brief Clear a socket descriptor from a file descriptor set.
 *
 * This function clears the socket descriptor "sockfd" from the fd_set pointed
 * to by "set".
 *
 * @param sockfd The socket descriptor to be cleared from the set.
 * @param set Pointer to the fd_set from which the descriptor is cleared.
 *
 * @example
 * tcp_fd_set_clear(my_socket, &my_set);
 */
void tcp_fd_set_clear(int sockfd, fd_set *set);

    #define M_CS "\033[1\5931m"
    #define M_CE "\033[0m"
    #define M_NAME M_CS "[LIBTCP DEBUG] [ERROR][%s]" M_CE
    #define ERROR_LOG_MESSAGE M_NAME ", [ERRNO: %d], [ERRMSG: %s]\n"

static inline int handle_error(int err, const char *errmsg)
{
    if (err == -1) {
    #if defined(DEBUG)
        printf(ERROR_LOG_MESSAGE, errmsg, errno, strerror(errno));
    #endif
        perror(errmsg);
        return -1;
    }
    return 0;
}

#endif /* !TCP_LIB_H_ */
