/*
** EPITECH PROJECT, 2023
** tcp-lib
** File description:
** basics
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "tcp_lib.h"

void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(tcp_lib, test_tcp_connect, .init = redirect_all_std)
{
    int sockfd = tcp_connect("127.0.0.1", 8080);
    cr_assert(sockfd < 0, "connect error: Connection refused");
    close(sockfd);
}

Test(tcp_lib, test_tcp_listen, .init = redirect_all_std)
{
    int sockfd = tcp_listen(8080, 5);
    cr_assert(sockfd >= 0, "Failed to set up server");
    close(sockfd);
}

Test(tcp_lib, test_tcp_accept, .init = redirect_all_std)
{
    int server_sockfd = tcp_listen(8080, 5);
    cr_assert(server_sockfd >= 0, "Failed to set up server");

    int client_sockfd = tcp_connect("127.0.0.1", 8080);
    cr_assert(client_sockfd >= 0, "Failed to connect to server");

    int accepted_sockfd = tcp_accept(server_sockfd);
    cr_assert(accepted_sockfd >= 0, "Failed to accept incoming connection");

    close(client_sockfd);
    close(accepted_sockfd);
    close(server_sockfd);
}

Test(tcp_lib, test_tcp_send_recv, .init = redirect_all_std)
{
    const char *msg = "Hello, server!";
    char buffer[1024];

    int server_sockfd = tcp_listen(8080, 5);
    cr_assert(server_sockfd >= 0, "Failed to set up server");

    int client_sockfd = tcp_connect("127.0.0.1", 8080);
    cr_assert(client_sockfd >= 0, "Failed to connect to server");

    int accepted_sockfd = tcp_accept(server_sockfd);
    cr_assert(accepted_sockfd >= 0, "Failed to accept incoming connection");

    ssize_t bytes_sent = tcp_send(client_sockfd, msg, strlen(msg));
    cr_assert((bytes_sent == (ssize_t)(strlen(msg))), "Failed to send all data to server");

    ssize_t bytes_received = tcp_recv(accepted_sockfd, buffer, sizeof(buffer) - 1);
    cr_assert(bytes_received == bytes_sent, "Received data size mismatch");

    buffer[bytes_received] = '\0';
    cr_assert(strcmp(msg, buffer) == 0, "Received data mismatch");

    close(client_sockfd);
    close(accepted_sockfd);
    close(server_sockfd);
}
