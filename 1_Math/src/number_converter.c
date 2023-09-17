#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "number_converter.h"
#include "utilities.h"

#define BASE 10

int str_to_int32(const char *p_input, number_t *p_output)
{
    int exit_code = E_FAILURE;

    if ((NULL == p_input) || (NULL == p_output))
    {
        print_error("NULL pointer passed as parameter.");
        goto END;
    }

    char *endptr = NULL;

    errno = 0;
    long result = strtol(p_input, &endptr, BASE);

    // Check if there was a reported error when the result is equal to 0
    if ((0 != errno) && (0 == result))
    {
        print_error("strtol() unknown error.");
        goto END;
    }

    // Check if there was a reported error when the result is equal to LONG_MAX
    if ((ERANGE == errno) && (LONG_MAX == result))
    {
        print_error("signed 32-bit overflow detected.");
        goto END;
    }

    // Check if the result is greater than INT32_MAX
    if (INT32_MAX < result)
    {
        print_error("signed 32-bit integer overflow detected.");
        goto END;
    }

    // Check if the result is less than INT32_MIN
    if (INT32_MIN > result)
    {
        print_error("signed 32-bit integer underflow detected.");
        goto END;
    }

    // Check if no digits were found
    if (endptr == p_input)
    {
        print_error("strtol() invalid input - no digits found.");
        goto END;
    }

    // Check if there are any extra characters by dereferencing the pointer to
    // the first character that can't be converted
    if ('\0' != *endptr)
    {
        print_error("strtol() invalid input - extra characters.");
        goto END;
    }

    p_output->signed_num = (int32_t)result;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int str_to_uint32(const char *p_input, number_t *p_output)
{
    int exit_code = E_FAILURE;

    if ((NULL == p_input) || (NULL == p_output))
    {
        print_error("NULL pointer passed as parameter.");
        goto END;
    }

    char *endptr = NULL;

    errno = 0;
    unsigned long result = strtoul(p_input, &endptr, BASE);

    // Check if there was a reported error when the result is equal to 0
    if ((0 != errno) && (0 == result))
    {
        print_error("strtol() unknown error.");
        goto END;
    }

    // Check if there was a reported error when the result is equal to ULONG_MAX
    if ((ERANGE == errno) && (ULONG_MAX == result))
    {
        print_error("unsigned 32-bit overflow detected.");
        goto END;
    }

    // Check if the result is greater than UINT32_MAX
    if (UINT32_MAX < result)
    {
        print_error("unsigned 32-bit integer overflow detected.");
        goto END;
    }

    // Check if no digits were found
    if (endptr == p_input)
    {
        print_error("strtol() invalid input - no digits found.");
        goto END;
    }

    // Check if there are any extra characters by dereferencing the pointer to
    // the first character that can't be converted
    if ('\0' != *endptr)
    {
        print_error("strtol() invalid input - extra characters.");
        goto END;
    }

    p_output->unsigned_num = (uint32_t)result;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

/*** end of file ***/
