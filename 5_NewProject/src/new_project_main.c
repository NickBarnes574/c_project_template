#include "signal_handler.h"
#include "socket_io.h"
#include "tcp_server_new.h"
#include "utilities.h"

#include <unistd.h>

#define MAX_BUFF_SIZE   14
#define PLACEHOLDER_NUM 10

typedef struct user_shared_data
{
    int placeholder;
} user_shared_data_t;

static void * process_request(void * arg_p);

int main(int argc, char ** argv)
{
    int                exit_code   = E_FAILURE;
    user_shared_data_t shared_data = { 0 };
    (void)argc;
    (void)argv;

    exit_code = signal_action_setup();
    if (E_SUCCESS != exit_code)
    {
        print_error("main(): Unable to setup signal handler.");
        goto END;
    }

    shared_data.placeholder = PLACEHOLDER_NUM;

    exit_code = start_server("31337", 4, process_request, NULL, &shared_data);
    if (E_SUCCESS != exit_code)
    {
        print_error("main(): Unable to run server.");
        goto END;
    }

END:
    return exit_code;
}

int echo(int client_fd)
{
    int     exit_code = E_FAILURE;
    uint8_t buffer[MAX_BUFF_SIZE];

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

static void * process_request(void * arg_p)
{
    int                  exit_code;
    client_data_t *      data_p        = NULL;
    user_shared_data_t * shared_data_p = NULL;
    int                  client_fd     = -1;

    if (NULL == arg_p)
    {
        print_error("process_request(): NULL argument passed.");
        goto END;
    }

    data_p        = (client_data_t *)arg_p;
    shared_data_p = (user_shared_data_t *)data_p->user_data_p;
    client_fd     = data_p->client_fd;

    printf("placeholder: %d\n", shared_data_p->placeholder);

    exit_code = echo(client_fd);
    if (E_SUCCESS != exit_code)
    {
        print_error("process_request(): Error processing echo request.");
        goto END;
    }

END:
    return NULL;
}
