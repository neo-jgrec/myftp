/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** error_handling
*/

#ifndef ERROR_HANDLING_H_
    #define ERROR_HANDLING_H_

    #include <stdbool.h>
    #ifndef fprintf
        #include <stdio.h>
    #endif

    /**
     * @brief Handles errors.
     *
     * This macro is used to handle errors.
     * The _Generic keyword is used to determine the type of the first argument
     * and call the appropriate function.
     *
     * @param err The error to handle.
     * @param msg The message to print if an error occurs.
     *
     * @return true if no error occured, false otherwise.
     *
     * @example
     * int fd = open("file.txt", O_RDONLY);
     * if (!error_handling(fd, "Failed to open file")) {
     *    exit(EXIT_FAILURE);
     * }
     * // File opened successfully, fd can be used to read/write data
     */
    #define error_handling(err, msg) _Generic((err), \
        int : error_handling_int, \
        char * : error_handling_char, \
        default: error_handling_default)(err, msg)

static inline int error_handling_int(int err, const char *msg) {
    if (err < 0) {
        fprintf(stderr, "%s\n", msg);
        return false;
    }
    return true;
}

static inline int error_handling_char(char *err, const char *msg)
{
    if (!err) {
        fprintf(stderr, "%s\n", msg);
        return false;
    }
    return true;
}

static inline int error_handling_default(void *err, const char *msg)
{
    if (!err) {
        fprintf(stderr, "%s\n", msg);
        return false;
    }
    return true;
}

#endif /* !ERROR_HANDLING_H_ */
