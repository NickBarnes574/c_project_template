#include <stdio.h>
#include <stdlib.h>

#include "math_functions_32_bit.h"
#include "utilities.h"

#define BIT_SIZE 32 // For use inside the rotate functions

/*****************
 *
 * int32_t functions
 *
 ******************/

// Covers 4.1.4: use proper declaration of int32_t
// Covers 4.1.6: perform addition using proper order of operations
// Covers 4.1.9: a function that is passed an argument by value
// Covers 4.1.9: a function that takes a pointer argument
// Covers 4.1.9: a function that returns a value using a return statement
// Covers 4.1.9: a function that modifies an output parameter through a pointer
int add_int32(int32_t operand_1, int32_t operand_2, int32_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    // Check if the result of potentially adding two positive numbers results in
    // a negative number due to an INT32_MAX overflow
    if ((0 < operand_2) && ((INT32_MAX - operand_2) < operand_1))
    {
        print_error("Overflow detected.");
        goto END;
    }

    // Check if the result of potentially adding two negative numbers produces
    // a positive number due to an INT32_MIN underflow
    if ((0 > operand_2) && ((INT32_MIN - operand_2) > operand_1))
    {
        print_error("Underflow detected.");
        goto END;
    }

    *p_result = operand_1 + operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

// Covers 4.1.6: perform subtraction using proper order of operations
int subtract_int32(int32_t operand_1, int32_t operand_2, int32_t *p_result)
{
    int exit_code = E_FAILURE;

    if (NULL == p_result)
    {
        goto END;
    }

    // Check if the result of potentially subtracting a negative number from a
    // postive number produces a negative number due to an INT32_MAX overflow
    if ((0 > operand_2) && ((INT32_MAX + operand_2) < operand_1))
    {
        print_error("Overflow detected.");
        goto END;
    }

    // Check if the result of potentially subtracting two negative numbers
    // produces a positive number due to an INT32_MIN underflow
    if ((0 < operand_2) && ((INT32_MIN + operand_2) > operand_1))
    {
        print_error("Underflow detected.");
        goto END;
    }

    *p_result = operand_1 - operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

// Covers 4.1.6: perform multiplication using proper order of operations
int multiply_int32(int32_t operand_1, int32_t operand_2, int32_t *p_result)
{
    int exit_code = E_FAILURE;
    int32_t check_quotient = 0;

    if (NULL == p_result)
    {
        goto END;
    }

    if ((0 < operand_1) && (0 < operand_2))
    {
        check_quotient = INT32_MAX / operand_2;

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
        check_quotient = INT32_MIN / operand_1;

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
        check_quotient = INT32_MIN / operand_2;

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
        check_quotient = INT32_MAX / operand_1;

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

// Covers 4.1.6: perform division using proper order of operations
int divide_int32(int32_t operand_1, int32_t operand_2, int32_t *p_result)
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
        goto END;
    }

    // Check for -1 on two's complement machines
    if ((-1 == operand_2) && (INT32_MIN == operand_1))
    {
        print_error("Overflow possible.");
        goto END;
    }

    *p_result = operand_1 / operand_2;

    exit_code = E_SUCCESS;
END:
    return exit_code;
}

// Covers 4.1.6: perform modulus using proper order of operations
int modulo_int32(int32_t operand_1, int32_t operand_2, int32_t *p_result)
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
    if ((-1 == operand_2) && (INT32_MIN == operand_1))
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
 * uint32_t functions
 *
 ******************/

// Covers 4.1.4: use proper declaration of uint32_t
int shift_left_uint32(uint32_t operand_1, uint32_t operand_2,
                      uint32_t *p_result)
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

int shift_right_uint32(uint32_t operand_1, uint32_t operand_2,
                       uint32_t *p_result)
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

int and_uint32(uint32_t operand_1, uint32_t operand_2, uint32_t *p_result)
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

int or_uint32(uint32_t operand_1, uint32_t operand_2, uint32_t *p_result)
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

int xor_uint32(uint32_t operand_1, uint32_t operand_2, uint32_t *p_result)
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

int rotate_left_uint32(uint32_t operand_1, uint32_t operand_2,
                       uint32_t *p_result)
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

int rotate_right_uint32(uint32_t operand_1, uint32_t operand_2,
                        uint32_t *p_result)
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
