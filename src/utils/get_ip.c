/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** get_ip
*/

#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

char *get_ip(void)
{
    char *ip = NULL;
    struct ifaddrs *ifaddr = NULL;
    struct ifaddrs *ifa = NULL;

    if (getifaddrs(&ifaddr) == -1)
        return (NULL);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr
            && ifa->ifa_addr->sa_family == AF_INET
            && strcmp(ifa->ifa_name, "lo") != 0) {
            ip = inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr);
            break;
        }
    }
    freeifaddrs(ifaddr);
    return (ip);
}
