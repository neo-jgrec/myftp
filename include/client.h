/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** client
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include <stdbool.h>
    #include <sys/queue.h>
    #include <sys/types.h>
    #include <unistd.h>
    #ifndef sockaddr_in
        #include <netinet/in.h>
    #endif

typedef enum {
    NO_TRANSFER,
    ACTIVE_TRANSFER,
    PASSIVE_TRANSFER
} transfer_t;

typedef struct client_s {
    int fd;
    pid_t pid;
    TAILQ_ENTRY(client_s) entries;
    struct sockaddr_in addr;
    char *username;
    char *password;
    char *cwd;
    transfer_t transfer;
    unsigned int data_port;
    int data_fd;
    char data_ip[16];
    char *ip;
    bool logged_in;
    struct sockaddr_in data_addr;
} client_t;

TAILQ_HEAD(client_head, client_s);

typedef struct command_s {
    char *name;
    int (*func)(client_t *client, char *arg);
    bool need_login;
} command_t;

// MANDATORY COMMANDS

/**
 * @brief Specify user for authentication.
 *
 * @param client The client to authenticate.
 * @param arg <SP> <username> <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. USER
 *
 * @example USER anonymous
 */
int user(client_t *client, char *arg);

/**
 * @brief Specify password for authentication.
 *
 * @param client The client to authenticate.
 * @param arg <SP> <password> <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. PASS
 *
 * @example PASS anonymous
 */
int pass(client_t *client, char *arg);

/**
 * @brief Change working directory.
 *
 * @param client The client to change working directory.
 * @param arg <SP> <pathname> <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. CWD
 *
 * @example CWD /home
 */
int cwd(client_t *client, char *arg);

/**
 * @brief Change working directory to parent directory.
 *
 * @param client The client to change working directory.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. CDUP
 *
 * @example CDUP
 */
int cdup(client_t *client, char *arg);

/**
 * @brief Disconnection.
 *
 * @param client The client to disconnect.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. QUIT
 *
 * @example QUIT
 */
int quit(client_t *client, char *arg);

/**
 * @brief Delete file.
 *
 * @param client The client to delete file.
 * @param arg <SP> <pathname> <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. DELE
 *
 * @example DELE /home/file
 */
int dele(client_t *client, char *arg);

/**
 * @brief Print working directory.
 *
 * @param client The client to print working directory.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. PWD
 *
 * @example PWD
 */
int pwd(client_t *client, char *arg);

/**
 * @brief Enter passive mode.
 *
 * @param client The client to enter passive mode.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. PASV
 *
 * @example PASV
 */
int pasv(client_t *client, char *arg);

/**
 * @brief Enter active mode.
 *
 * @param client The client to enter active mode.
 * @param arg <SP> <host-port> <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. PORT
 *
 * @example PORT 127,0,0,1,0,21
 */
int port(client_t *client, char *arg);

/**
 * @brief Print help information.
 *
 * @param client The client to print help information.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. HELP
 *
 * @example HELP
 */
int help(client_t *client, char *arg);

/**
 * @brief Do nothing.
 *
 * @param client The client to do nothing.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. NOOP
 *
 * @example NOOP
 */
int noop(client_t *client, char *arg);

/**
 * @brief Retrieve file.
 *
 * @param client The client to retrieve file.
 * @param arg <SP> <pathname> <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @example RETR /home/file
 */
int retr(client_t *client, char *arg);

/**
 * @brief Store file.
 *
 * @param client The client to store file.
 * @param arg <SP> <pathname> <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @example STOR /home/file
 */
int stor(client_t *client, char *arg);

/**
 * @brief List files in the current working directory.
 *
 * @param client The client to list files.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @example LIST
 */
int list(client_t *client, char *arg);

// BONUS COMMANDS

/**
 * @brief System information.
 *
 * @param client The client to print system information.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @see RFC 959 - 4.1.1. SYST
 *
 * @example SYST
 */
int syst(client_t *client, char *arg);

/**
 * @brief Abort.
 *
 * @param client The client to abort.
 * @param arg <CRLF>
 *
 * @return int
 *
 * @retval 0 If success.
 * @retval 1 If error.
 *
 * @example ABOR
 */
int abor(client_t *client, char *arg);

static const command_t commands[] = {
    {"USER", &user, false},
    {"PASS", &pass, false},
    {"CWD", &cwd, true},
    {"CDUP", &cdup, true},
    {"QUIT", &quit, false},
    {"DELE", &dele, true},
    {"PWD", &pwd, true},
    {"PASV", &pasv, true},
    {"PORT", &port, true},
    {"HELP", &help, false},
    {"NOOP", &noop, false},
    {"RETR", &retr, true},
    {"STOR", &stor, true},
    {"LIST", &list, true},
    {"SYST", &syst, false},
    {"ABOR", &abor, false},
    {NULL, NULL, false}
};

// Default registered users
typedef struct user_s {
    char *username;
    char *password;
} user_t;

static const user_t default_users[] = {
    {"Anonymous", ""},
    {NULL, NULL}
};

#endif /* !CLIENT_H_ */
