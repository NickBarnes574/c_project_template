#define _GNU_SOURCE

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utilities.h"

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define HELLO_WORLD "Hello World"
#define ONE_SECOND  1

int main()
{
    int                exit_code = E_FAILURE;
    int                server_fd;
    int                len;
    struct sockaddr_in server_addr;
    char               buffer[BUFFER_SIZE] = { 0 };
    const char *       message_p           = HELLO_WORLD;

    // Create socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (E_SUCCESS != server_fd)
    {
        perror("Socket creation failed");
        goto END;
    }

    // Set server address
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (E_SUCCESS !=
        bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        perror("Bind failed");
        goto END;
    }

    printf("UDP server is running on port %d\n", SERVER_PORT);

    while (1)
    {
        snprintf(buffer, BUFFER_SIZE, "%s\n", message_p);
        len = strnlen(buffer, BUFFER_SIZE);

        // Send message
        if (sendto(server_fd,
                   buffer,
                   len,
                   0,
                   (struct sockaddr *)&server_addr,
                   sizeof(server_addr)) < 0)
        {
            perror("Sendto failed");
            break;
        }

        printf("Sent message: %s", buffer);

        // Wait for 1 second
        sleep(ONE_SECOND);
    }

    exit_code = E_SUCCESS;

END:
    if (server_fd >= 0)
    {
        close(server_fd);
    }

    return exit_code;
}
