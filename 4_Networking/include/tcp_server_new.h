/**
 * @file tcp_server_new.h
 *
 * @brief
 */
#ifndef _TCP_SERVER_NEW_H
#define _TCP_SERVER_NEW_H

#include "threadpool.h"

#define SIG_SHUTDOWN 1

typedef struct config config_t;

typedef struct tcp_server_job
{
    void * (*client_function)(void *);
    void (*free_function)(void *);
    void * args_p;
} tcp_server_job_t;

typedef struct server
{
    int            listening_socket;
    threadpool_t * threadpool_p;
    void * (*client_request_handler)(void *);
    void (*client_data_free_func)(void *);
    config_t * settings_p;
} server_t;

server_t * init_server(char * port_p,
                       size_t max_connections,
                       void * (*client_request_handler)(void *),
                       void (*client_data_free_func)(void *));

int run_server(char * port_p,
               size_t max_connections,
               void * (*client_request_handler)(void *),
               void (*client_data_free_func)(void *));

int destroy_server(server_t ** server_p);

#endif /* _TCP_SERVER_NEW_H */

/*** end of file ***/
