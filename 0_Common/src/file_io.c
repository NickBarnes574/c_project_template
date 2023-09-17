#include <stdint.h>
#include <unistd.h>

#include "file_io.h"
#include "utilities.h"

/**
 * @brief Limits the number of bytes that can be read or written to at one time
 *
 * @param num_bytes The number of bytes to process
 * @param bytes_processed The number of bytes that have already been processed
 * @return size_t Returns the total number of bytes to be processed
 */
static size_t limit_bytes(size_t num_bytes, size_t bytes_processed);

// Covers 4.8.1: Demonstrate the ability to handle partial reads and writes
// during serialization and de-serialization
int read_bytes(int read_fd, void *buffer, size_t num_bytes)
{
    int exit_code = E_FAILURE;
    size_t bytes_read = 0;
    ssize_t check = E_FAILURE;
    size_t bytes_to_read = 0;

    if (NULL == buffer)
    {
        print_error("NULL argument passed.");
        goto END;
    }

    if (num_bytes == bytes_read)
    {
        print_error("Nothing to read.");
        goto END;
    }

    while (bytes_read < num_bytes)
    {
        // Only allow MAX_BYTES to be read at a time.
        bytes_to_read = limit_bytes(num_bytes, bytes_read);

        check = read(read_fd, ((uint8_t *)buffer + bytes_read), bytes_to_read);

        if (E_FAILURE == check)
        {
            print_error("read() error.");
            goto END;
        }

        if (E_SUCCESS == check)
        {
            break;
        }

        bytes_read += check;
    }

    if (num_bytes != bytes_read)
    {
        print_error("Incorrect number of bytes were read.");
    }

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int write_bytes(int write_fd, void *buffer, size_t num_bytes)
{
    int exit_code = E_FAILURE;
    size_t bytes_written = 0;
    ssize_t check = E_FAILURE;
    size_t bytes_to_write = 0;

    if (NULL == buffer)
    {
        print_error("NULL argument passed.");
        goto END;
    }

    if (num_bytes == bytes_written)
    {
        print_error("Nothing to write.");
        goto END;
    }

    while (bytes_written < num_bytes)
    {
        // Only allow MAX_BYTES to be written at a time.
        bytes_to_write = limit_bytes(num_bytes, bytes_written);

        check = write(write_fd, ((uint8_t *)buffer + bytes_written),
                      bytes_to_write);

        if (E_FAILURE == check)
        {
            print_error("write() error.");
            goto END;
        }

        if (E_SUCCESS == check)
        {
            break;
        }

        bytes_written += check;
    }

    if (num_bytes != bytes_written)
    {
        print_error("Incorrect number of bytes were written.");
    }

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int close_fd(int file_descriptor)
{
    int exit_code = E_FAILURE;

    if (MIN_FD > file_descriptor)
    {
        print_error(
            "Invalid file descriptor. Attempting to close 'stdin', 'stdout', "
            "or 'stderr' would result in unexpected behavior.\n");
        goto END;
    }

    exit_code = close(file_descriptor);
    if (E_SUCCESS != exit_code)
    {
        print_error("close() failed.");
    }

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

size_t limit_bytes(size_t num_bytes, size_t bytes_processed)
{
    size_t bytes_to_process = 0;

    if (MAX_BYTES < (num_bytes - bytes_processed))
    {
        bytes_to_process = MAX_BYTES;
    }
    else
    {
        bytes_to_process = (num_bytes - bytes_processed);
    }

    return bytes_to_process;
}

/*** end of file ***/
