#include <stdio.h>
#include <stdlib.h>

#include "math_functions_64_bit.h"
#include "utilities.h"

#define BIT_SIZE 64 // For use inside the rotate functions

/*****************
 *
 * int64_t functions
 *
 ******************/

int add_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    // Check if the result of potentially adding two positive numbers results in
    // a negative number due to an INT64_MAX overflow
    if ((0 < operand_2) && ((INT64_MAX - operand_2) < operand_1))
    {
        print_error("Overflow detected.");
        goto END;
    }

    // Check if the result of potentially adding two negative numbers produces
    // a positive number due to an INT64_MIN underflow
    if ((0 > operand_2) && ((INT64_MIN - operand_2) > operand_1))
    {
        print_error("Underflow detected.");
        goto END;
    }

    *p_result = operand_1 + operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int subtract_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    // Check if the result of potentially subtracting a negative number from a
    // postive number produces a negative number due to an INT64_MAX overflow
    if ((0 > operand_2) && ((INT64_MAX + operand_2) < operand_1))
    {
        print_error("Overflow detected.");
        goto END;
    }

    // Check if the result of potentially subtracting two negative numbers
    // produces a positive number due to an INT64_MIN underflow
    if ((0 < operand_2) && ((INT64_MIN + operand_2) > operand_1))
    {
        print_error("Underflow detected.");
        goto END;
    }

    *p_result = operand_1 - operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int multiply_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result)
{
    int exit_code = E_FAILURE;
    int64_t check_quotient = 0;

    if (NULL == p_result)
    {
        goto END;
    }

    if ((0 < operand_1) && (0 < operand_2))
    {
        check_quotient = INT64_MAX / operand_2;

        // When both operands are positive, the product of operand_1 and
        // operand_2 will cause an overflow when operand_1 is greater than
        // check_quotient
        if (operand_1 > check_quotient)
        {
            print_error("Overflow detected.");
            goto END;
        }
    }

    if ((0 < operand_1) && (0 > operand_2))
    {
        check_quotient = INT64_MIN / operand_1;

        // When operand_1 is positive and operand_2 is negative, the product of
        // operand_1 and operand_2 will cause and underflow when operand_2 is
        // less than check_quotient
        if (operand_2 < check_quotient)
        {
            print_error("Underflow detected.");
            goto END;
        }
    }

    if ((0 > operand_1) && (0 < operand_2))
    {
        check_quotient = INT64_MIN / operand_2;

        // When operand_1 is negative and operand_2 is positive, the product of
        // operand_1 and operand_2 will cause an underflow when operand_2 is
        // less than check_quotient
        if (operand_2 < check_quotient)
        {
            print_error("Underflow detected.");
            goto END;
        }
    }

    if ((0 > operand_1) && (0 > operand_2))
    {
        check_quotient = INT64_MAX / operand_1;

        // When both operands are negative and operand_1 is not 0, the product
        // of operand_1 and operand_2 will cause an overflow when operand_2 is
        // less than check_quotient
        if ((operand_1 != 0) && (operand_2 < check_quotient))
        {
            print_error("Overflow detected.");
            goto END;
        }
    }

    *p_result = operand_1 * operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int divide_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    // Check for division by 0
    if (0 == operand_2)
    {
        print_error("Divide by zero error.");
        *p_result = 0;
        goto END;
    }

    // Check for -1 on two's complement machines
    if ((-1 == operand_2) && (INT64_MIN == operand_1))
    {
        print_error("Overflow possible.");
        goto END;
    }

    *p_result = operand_1 / operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int modulo_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    // Check for division by 0
    if (0 == operand_2)
    {
        print_error("Modulo by zero error.");
        goto END;
    }

    // Check for -1 on two's complement machines
    if ((-1 == operand_2) && (INT64_MIN == operand_1))
    {
        print_error("Overflow possible.");
        goto END;
    }

    *p_result = operand_1 % operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

/*****************
 *
 * uint64_t functions
 *
 ******************/

int shift_left_uint64(uint64_t operand_1, uint64_t operand_2,
                      uint64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    *p_result = operand_1 << operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int shift_right_uint64(uint64_t operand_1, uint64_t operand_2,
                       uint64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    *p_result = operand_1 >> operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int and_uint64(uint64_t operand_1, uint64_t operand_2, uint64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    *p_result = operand_1 & operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int or_uint64(uint64_t operand_1, uint64_t operand_2, uint64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    *p_result = operand_1 | operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int xor_uint64(uint64_t operand_1, uint64_t operand_2, uint64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    *p_result = operand_1 ^ operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int rotate_left_uint64(uint64_t operand_1, uint64_t operand_2,
                       uint64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    *p_result =
        (operand_1 << operand_2) | (operand_1 >> (BIT_SIZE - operand_2));

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

int rotate_right_uint64(uint64_t operand_1, uint64_t operand_2,
                        uint64_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    *p_result =
        (operand_1 >> operand_2) | (operand_1 << (BIT_SIZE - operand_2));

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

/*** end of file ***/
