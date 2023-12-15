/**
 * @file tcp_server.h
 *
 * @brief Server-side logic for the application.
 */
#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "threadpool.h"

/**
 * @brief Indicator for server shutdown.
 *
 * This define value is returned by certain functions to indicate
 * that a shutdown signal has been received and the server should
 * terminate its operations gracefully.
 */
#define SHUTDOWN 1

typedef int (*request_handler_t)(int);

/**
 * @struct server_cfg
 * @brief Holds the configuration settings for the server.
 *
 * This structure stores all the necessary information for setting up
 * and running the server, such as the address to bind to, the
 * listening socket, and client-specific details.
 */
typedef struct server_cfg server_cfg_t;

int run_server(size_t            num_threads,
               char *            port_p,
               request_handler_t handler_func);

#endif /* _SERVER_H */

/*** end of file ***/
