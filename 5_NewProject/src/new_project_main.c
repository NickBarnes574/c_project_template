#include "signal_handler.h"
#include "socket_io.h"
#include "tcp_server_new.h"
#include "utilities.h"

#define MAX_BUFF_SIZE 14

static int echo(int client_fd);

int main(int argc, char ** argv)
{
    int        exit_code = E_FAILURE;
    server_t * server_p  = NULL;
    (void)argc;
    (void)argv;

    exit_code = signal_action_setup();
    if (E_SUCCESS != exit_code)
    {
        print_error("main(): Unable to setup signal handler.");
        goto END;
    }

    // exit_code = start_server(4, "31337", echo);
    server_p = init_server("31337", 4, echo);
    if (NULL == server_p)
    {
        print_error("main(): Failed to initialize server.");
        goto END;
    }

    exit_code = start_server(server_p);
    if (E_SUCCESS != exit_code)
    {
        print_error("main(): Failed to start server.");
        goto END;
    }

END:
    return exit_code;
}

static int echo(int client_fd)
{
    uint8_t buffer[MAX_BUFF_SIZE];
    int     exit_code;

    // Receive data from the client
    exit_code = recv_data(client_fd, buffer, sizeof(buffer));
    if (E_SUCCESS != exit_code)
    {
        print_error("Failed to receive data from the client.");
        goto END;
    }

    printf("%s\n", buffer);

    // Send the received data back to the client
    exit_code = send_data(client_fd, buffer, sizeof(buffer));
    if (E_SUCCESS != exit_code)
    {
        print_error("Failed to send data back to the client.");
        goto END;
    }

END:
    return exit_code;
}
