/**
 * @file tcp_server_new.h
 *
 * @brief
 */
#ifndef _TCP_SERVER_NEW_H
#define _TCP_SERVER_NEW_H

#include "threadpool.h"

#define SHUTDOWN 1

typedef int (*request_handler_t)(int);

typedef struct config config_t;

typedef struct server
{
    int               listening_socket;
    threadpool_t *    threadpool_p;
    request_handler_t client_request_p;
    config_t *        settings_p;
} server_t;

server_t * init_server(char *            port_p,
                       size_t            max_connections,
                       request_handler_t client_request_p);

int start_server(server_t * server_p);
int stop_server();

#endif /* _TCP_SERVER_NEW_H */

/*** end of file ***/
