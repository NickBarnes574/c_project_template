/**
 * @file utilities.h
 *
 * @brief Module for utilities for wide-variety of use cases
 */

#ifndef _UTILITIES_H
#define _UTILITIES_H

// Exit codes
#define E_SUCCESS 0
#define E_FAILURE -1

/**
 * @brief Prints a custom error message to `stderr` using `fprintf()`
 *
 * @param p_message The message to print
 * @return void - Returns nothing
 */
void print_error(const char *p_message);

/**
 * @brief Prints a custom error message to 'stderr' in addition to an 'errno'
 * string
 *
 * @param p_message The message to print
 * @param p_errno A `strerr(errno)` string
 * @return void - Returns nothing
 */
void print_errno(const char *p_message, char *p_errno);

#endif /* _UTILITIES_H */

/*** end of file ***/
