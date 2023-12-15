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
#include "tcp_server_new.h"
#include "utilities.h"

#define INVALID_SOCKET          (-1) // Indicates an invalid socket descriptor
#define BACKLOG_SIZE            10 // Maximum number of pending client connections
#define MAX_CLIENT_ADDRESS_SIZE 100 // Size for storing client address strings

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
        current_address;  // Pointer used to traverse 'address_list'
    int listening_socket; // Listening socket for the server
    struct sockaddr_storage client_address; // Stores client address
    int                     client_fd;      // Socket for accepting connections
    socklen_t               client_len; // Length of client address structure
};

typedef struct client_args
{
    int               client_fd;
    request_handler_t handler_func;
} client_args_t;

static int    configure_server_address(config_t * config_p, char * port_p);
static int    create_listening_socket(config_t * config_p);
static int    accept_new_connection(config_t * config_p);
static void   print_client_address(config_t * config_p);
static void * handle_client_request(void * args_p);

server_t * init_server(char *            port_p,
                       size_t            max_connections,
                       request_handler_t client_request_p)
{
    int            exit_code    = E_FAILURE;
    threadpool_t * threadpool_p = NULL;
    config_t *     config_p     = NULL;
    server_t *     server_p     = NULL;

    if ((NULL == port_p) || (NULL == client_request_p))
    {
        print_error("init_server(): NULL argument passed.");
        goto END;
    }

    if (2 > max_connections)
    {
        print_error("init_server(): Max connections must be 2 or more.");
        goto END;
    }

    threadpool_p = threadpool_create(max_connections);
    if (NULL == threadpool_p)
    {
        print_error("init_server(): Unable to create threadpool.");
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

    // Activate listening mode for the server's socket, allowing it to queue up
    // to 'BACKLOG_SIZE' connection requests at a time.
    errno     = 0;
    exit_code = listen(config_p->listening_socket, BACKLOG_SIZE);
    if (E_SUCCESS != exit_code)
    {
        fprintf(stderr, "listen() failed. (%s)\n", strerror(errno));
        goto END;
    }

    server_p = calloc(1, sizeof(server_t));
    if (NULL == server_p)
    {
        print_error("init_server(): server_p - CMR failure.");
        exit_code = E_FAILURE;
        goto END;
    }

    server_p->listening_socket = config_p->listening_socket;
    server_p->threadpool_p     = threadpool_p;
    server_p->client_request_p = client_request_p;
    server_p->settings_p       = config_p;

END:
    if (E_SUCCESS != exit_code)
    {
        free(config_p);
        config_p = NULL;

        threadpool_destroy(&threadpool_p);
        threadpool_p = NULL;

        free(server_p);
        server_p = NULL;
    }

    return server_p;
}

int start_server(server_t * server_p)
{
    int exit_code = E_FAILURE;

    if (NULL == server_p)
    {
        print_error("start_server(): NULL server.");
        goto END;
    }

    if (NULL == server_p->threadpool_p)
    {
        print_error("start_server(): NULL threadpool.");
        goto END;
    }

    if (NULL == server_p->client_request_p)
    {
        print_error("start_server(): NULL client request function.");
        goto END;
    }

    if (NULL == server_p->settings_p)
    {
        print_error("start_server(): NULL configuration settings.");
        goto END;
    }

    printf("Waiting for client connections...\n");

    // Constantly monitor for any incoming connections
    for (;;)
    {
        if ((signal_flag_g == SIGINT) || (signal_flag_g) == SIGUSR1)
        {
            printf("\nShutdown signal received.\n");
            exit_code = SHUTDOWN;
            goto END;
        }

        exit_code = accept_new_connection(server_p->settings_p);
        if (E_SUCCESS != exit_code)
        {
            print_error("start_server(): Unable to accept new connection.");
            goto END;
        }

        client_args_t * args_p = calloc(1, sizeof(client_args_t));
        if (NULL == args_p)
        {
            print_error("start_server(): args_p - CMR failure.");
            goto END;
        }

        args_p->client_fd    = server_p->settings_p->client_fd;
        args_p->handler_func = server_p->client_request_p;

        // Queue up a new client job to be processed
        exit_code = threadpool_add_job(
            server_p->threadpool_p, handle_client_request, NULL, args_p);
        if (E_SUCCESS != exit_code)
        {
            print_error("start_server(): Unable to add job to threadpool.");
            goto END;
        }

        print_client_address(server_p->settings_p);
    }

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

static int accept_new_connection(config_t * config_p)
{
    int exit_code        = E_FAILURE;
    config_p->client_len = sizeof(config_p->client_address);

    // Attempt to accept a new client connection using the listening socket.
    // If the accept() function returns an invalid client fd, check for shutdown
    // signals (SIGINT or SIGUSR1).
    errno               = 0;
    config_p->client_fd = accept(config_p->listening_socket,
                                 (struct sockaddr *)&config_p->client_address,
                                 &config_p->client_len);
    if (INVALID_SOCKET >= config_p->client_fd)
    {
        if ((signal_flag_g == SIGINT) || (signal_flag_g == SIGUSR1))
        {
            printf("\nShutdown signal received.\n");
            exit_code = SHUTDOWN;
            goto END;
        }

        perror("accept_new_connection(): accept() failed.");
        exit_code = E_FAILURE;
        goto END;
    }

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

static int configure_server_address(config_t * config_p, char * port_p)
{
    int exit_code = E_FAILURE;

    if ((NULL == config_p) || (NULL == port_p))
    {
        print_error("configure_server_address(): NULL argument passed.");
        goto END;
    }

    // Setup a TCP server address using IPV4
    config_p->hints = (struct addrinfo) { .ai_family   = AF_INET,     // IPV4
                                          .ai_socktype = SOCK_STREAM, // TCP
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
            /**
             * NOTE: Barr-C generally discourages the use of the `continue`
             * keyword, but I think it makes sense in this case. Using flags and
             * additional conditional statements in order to factor out the
             * `continue` keyword could diminish readability.
             */
            continue;
        }

        errno     = 0;
        exit_code = bind(config_p->listening_socket,
                         config_p->current_address->ai_addr,
                         config_p->current_address->ai_addrlen);
        if (E_SUCCESS == exit_code)
        {
            break; // Successfully bound socket
        }

        // Close the socket if unsuccessful
        close(config_p->listening_socket);
        perror("configure_server_address(): bind() failed.");
    }

    if (NULL == config_p->current_address)
    {
        // None of the addresses succeeded
        exit_code = E_FAILURE;
    }
    else
    {
        exit_code = E_SUCCESS;
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
    errno                      = 0;
    config_p->listening_socket = socket(config_p->current_address->ai_family,
                                        config_p->current_address->ai_socktype,
                                        config_p->current_address->ai_protocol);
    if (INVALID_SOCKET >= config_p->listening_socket)
    {
        perror("create_listening_socket(): socket() failed.");
        exit_code = E_FAILURE;
        goto END;
    }

    // Enable the SO_REUSEADDR option to reuse the local address
    sock_opt_check = setsockopt(config_p->listening_socket,
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

static void print_client_address(config_t * config_p)
{
    char address_buffer[MAX_CLIENT_ADDRESS_SIZE];
    getnameinfo((struct sockaddr *)&config_p->client_address,
                config_p->client_len,
                address_buffer,
                sizeof(address_buffer),
                0,
                0,
                NI_NUMERICHOST);
    printf("Connection established - [%s]\n", address_buffer);
}

static void * handle_client_request(void * args_p)
{
    int             exit_code  = E_FAILURE;
    client_args_t * new_args_p = (client_args_t *)args_p;

    exit_code = new_args_p->handler_func(new_args_p->client_fd);
    if (E_SUCCESS != exit_code)
    {
        print_error("Error handling client request.");
        goto END;
    }

END:
    if (NULL != new_args_p)
    {
        close(new_args_p->client_fd);
    }
    free(new_args_p);
    return NULL;
}

/*** end of file ***/
