/**
 * @file udp_server.h
 *
 * @brief
 */
#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

typedef struct config config_t;

typedef void * (*udp_request_t)(void *);
typedef void (*custom_free_t)(void *);

typedef struct udp_server
{
    int           server_socket;
    udp_request_t request_handler;
    custom_free_t custom_free;
    config_t *    settings_p;
} udp_server_t;

typedef struct udp_data
{
    void * user_data_p;
} udp_data_t;

int start_udp_server(char *        port_p,
                     udp_request_t request_handler,
                     custom_free_t custom_free,
                     void *        data_p);

#endif /* _UDP_SERVER_H */

/*** end of file ***/
