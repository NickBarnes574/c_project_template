#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"

void print_error(const char *p_message)
{
    /* [ACTION]: Suppressing fprintf() error.
     * [JUSTIFICATION]: Result inconsequential to any further operations.
     */
    // NOLINTNEXTLINE
    fprintf(stderr, "[ERROR]: %s\n", p_message);
}

void print_errno(const char *p_message, char *p_errno)
{
    /* [ACTION]: Suppressing fprintf() error.
     * [JUSTIFICATION]: Result inconsequential to any further operations.
     */
    // NOLINTNEXTLINE
    fprintf(stderr, "[ERROR]: %s\n[errno]: %s\n", p_message, p_errno);
}

/*** end of file ***/
