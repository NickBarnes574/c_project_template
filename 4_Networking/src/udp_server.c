#define _GNU_SOURCE

#include <arpa/inet.h> // bind(), accept()
#include <errno.h>     // Access 'errno' global variable
#include <netdb.h>     // getaddrinfo() struct
#include <stdio.h>     // printf(), fprintf()
#include <stdlib.h>    // calloc(), free()
#include <string.h>    // strerror()
#include <unistd.h>    // close()

#include "signal_handler.h"
#include "socket_io.h"
#include "udp_server.h"
#include "utilities.h"

#define INVALID_SOCKET (-1) // Indicates an invalid socket descriptor

/**
 * @struct config
 * @brief  Holds the configuration settings for the server.
 *
 * This structure stores all the necessary information for setting up
 * and running the server, such as the address to bind to, the
 * listening socket, and client-specific details.
 */
struct config
{
    struct addrinfo   hints;        // Specifies criteria for address selection
    struct addrinfo * address_list; // Pointer to linked list of address info
    struct addrinfo *
        current_address; // Pointer used to traverse 'address_list'
    int server_socket;   // Listening socket for the server
    struct sockaddr_storage client_address; // Stores client address
    int                     client_fd;      // Socket for accepting connections
    socklen_t               client_len; // Length of client address structure
};

static int configure_server_address(config_t * config_p, char * port_p);
static udp_server_t * init_server(char *        port_p,
                                  udp_request_t request_handler,
                                  custom_free_t custom_free);
static int            create_listening_socket(config_t * config_p);
static int            destroy_udp_server(udp_server_t ** server_p);

int start_udp_server(char *        port_p,
                     udp_request_t request_handler,
                     custom_free_t custom_free,
                     void *        data_p)
{
    int            exit_code = E_FAILURE;
    udp_server_t * server_p  = NULL;

    if ((NULL == port_p) || (NULL == request_handler) || (NULL == data_p))
    {
        print_error("start_udp_server(): NULL argument passed.");
        goto END;
    }

    server_p = init_server(port_p, request_handler, custom_free);
    if (NULL == server_p)
    {
        print_error("start_udp_server(): Failed to initialize server.");
        goto END;
    }

    for (;;)
    {
        if ((signal_flag_g == SIGINT) || (signal_flag_g) == SIGUSR1)
        {
            printf("\nShutdown signal received.\n");
            goto SHUTDOWN;
        }

        // Process the request
        request_handler(data_p);

        // Peform a cleanup if necessary
        if (NULL != custom_free)
        {
            custom_free(data_p);
        }
    }

SHUTDOWN:
    exit_code = destroy_udp_server(&server_p);
    if (E_SUCCESS != exit_code)
    {
        print_error("start_udp_server(): Failed to destroy server.");
        goto END;
    }

END:
    return exit_code;
}

static int destroy_udp_server(udp_server_t ** server_p)
{
    int exit_code = E_FAILURE;
    if ((NULL == server_p) || (NULL == (*server_p)))
    {
        print_error("destroy_udp_server(): NULL argument passed.");
        goto END;
    }

    close((*server_p)->server_socket);

    free((*server_p)->settings_p);
    (*server_p)->settings_p = NULL;

    free(*server_p);
    *server_p = NULL;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

static udp_server_t * init_server(char *        port_p,
                                  udp_request_t request_handler,
                                  custom_free_t custom_free)
{
    int            exit_code = E_FAILURE;
    config_t *     config_p  = NULL;
    udp_server_t * server_p  = NULL;

    if ((NULL == port_p) || (NULL == request_handler))
    {
        print_error("init_server(): NULL argument passed.");
        goto END;
    }

    config_p = calloc(1, sizeof(config_t));
    if (NULL == config_p)
    {
        print_error("init_server(): config_p - CMR failure.");
        goto END;
    }

    exit_code = configure_server_address(config_p, port_p);
    if (E_SUCCESS != exit_code)
    {
        print_error("init_server(): Unable to configure server address.");
        goto END;
    }

    server_p = calloc(1, sizeof(udp_server_t));
    if (NULL == server_p)
    {
        print_error("init_server(): server_p - CMR failure.");
        exit_code = E_FAILURE;
        goto END;
    }

    server_p->server_socket   = config_p->server_socket;
    server_p->request_handler = request_handler;
    server_p->custom_free     = custom_free;
    server_p->settings_p      = config_p;

END:
    if (E_SUCCESS != exit_code)
    {
        free(config_p);
        config_p = NULL;

        free(server_p);
        server_p = NULL;
    }

    return server_p;
}

static int configure_server_address(config_t * config_p, char * port_p)
{
    int exit_code = E_FAILURE;

    if ((NULL == config_p) || (NULL == port_p))
    {
        print_error("configure_server_address(): NULL argument passed.");
        goto END;
    }

    // Setup a UDP server address using IPV4
    config_p->hints = (struct addrinfo) { .ai_family   = AF_INET,    // IPV4
                                          .ai_socktype = SOCK_DGRAM, // UDP
                                          .ai_flags = AI_PASSIVE, // For binding
                                          .ai_protocol  = 0,
                                          .ai_canonname = NULL,
                                          .ai_addr      = NULL,
                                          .ai_next      = NULL };

    // Get a list of address structures and store them in 'config->address_list'
    exit_code =
        getaddrinfo(NULL, port_p, &config_p->hints, &config_p->address_list);
    if (E_SUCCESS != exit_code)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(exit_code));
        goto END;
    }

    // Try each address, attempting to create a socket and bind
    for (config_p->current_address = config_p->address_list;
         config_p->current_address != NULL;
         config_p->current_address = config_p->current_address->ai_next)
    {
        exit_code = create_listening_socket(config_p);
        if (E_SUCCESS != exit_code)
        {
            continue; // Try the next address
        }

        errno     = 0;
        exit_code = bind(config_p->server_socket,
                         config_p->current_address->ai_addr,
                         config_p->current_address->ai_addrlen);
        if (E_SUCCESS == exit_code)
        {
            break; // Successfully bound socket
        }

        // Close the socket if unsuccessful
        close(config_p->server_socket);
        perror("configure_server_address(): bind() failed.");
    }

    if (NULL == config_p->current_address)
    {
        exit_code = E_FAILURE; // None of the addresses succeeded
    }
    else
    {
        exit_code = E_SUCCESS; // Successfully configured and bound
    }

END:
    freeaddrinfo(config_p->address_list);
    return exit_code;
}

static int create_listening_socket(config_t * config_p)
{
    int exit_code      = E_FAILURE;
    int sock_opt_check = 0; // To check the return value of setsockopt
    int optval         = 1; // Enable SO_REUSEADDR

    if (NULL == config_p)
    {
        print_error("NULL argument passed.");
        goto END;
    }

    // Create a new socket
    errno                   = 0;
    config_p->server_socket = socket(config_p->current_address->ai_family,
                                     config_p->current_address->ai_socktype,
                                     config_p->current_address->ai_protocol);
    if (INVALID_SOCKET >= config_p->server_socket)
    {
        perror("create_listening_socket(): socket() failed.");
        exit_code = E_FAILURE;
        goto END;
    }

    // Enable the SO_REUSEADDR option to reuse the local address
    sock_opt_check = setsockopt(config_p->server_socket,
                                SOL_SOCKET,
                                SO_REUSEADDR,
                                &optval,
                                sizeof(optval));
    if (0 > sock_opt_check)
    {
        perror("create_listening_socket(): setsockopt() failed.");
        exit_code = E_FAILURE;
        goto END;
    }

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

/*** end of file ***/
