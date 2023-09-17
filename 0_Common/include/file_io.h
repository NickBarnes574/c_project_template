/**
 * @file file_io.h
 *
 * @brief A module for file I/O functions
 */
#ifndef _FILE_IO_H
#define _FILE_IO_H

#define MIN_FD 3 // The lowest allowable user-defined file descriptor
#define MAX_BYTES (size_t)4096 // Max number of bytes to read or write at once

/**
 * @brief Safely reads 'n' number of bytes from a file descriptor into a buffer.
 *
 * @param read_fd The file descriptor to read from
 * @param buffer The buffer to read into
 * @param num_bytes The number of bytes to read
 * @return int - Returns 0 on success state, -1 on error state
 */
int read_bytes(int read_fd, void *buffer, size_t num_bytes);

/**
 * @brief Safely writes 'n' number of bytes from a file descriptor into a
 * buffer.
 *
 * @param write_fd The file descriptor to write from
 * @param buffer The buffer to write to
 * @param num_bytes The number of bytes to write
 * @return int - Returns 0 on success state, -1 on error state
 */
int write_bytes(int write_fd, void *buffer, size_t num_bytes);

/**
 * @brief Safely closes user-defined file descriptors, avoiding 'stdin',
 * 'stdout', and 'stderr'.
 *
 * @param file_descriptor The file descriptor to close
 * @return int - Returns 0 on success state, -1 on error state
 */
int close_fd(int file_descriptor);

#endif /* _FILE_IO_H */

/*** end of file ***/
