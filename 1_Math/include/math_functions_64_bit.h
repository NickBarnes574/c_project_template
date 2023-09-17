/**
 * @file math_functions.h
 *
 * @brief Module for performing signed and unsigned 64-bit math functions
 */

#ifndef _MATH_FUNCTIONS_H
#define _MATH_FUNCTIONS_H

#include <limits.h> // INT_MAX, INT_MIN
#include <stdint.h> // int64_t

/**
 * @brief Performs addition on two int64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int add_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result);

/**
 * @brief Performs subtraction on two int64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int subtract_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result);

/**
 * @brief Performs multiplication on two int64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int multiply_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result);

/**
 * @brief Performs division between two int64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int divide_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result);

/**
 * @brief Performs a modulo between two int64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int modulo_int64(int64_t operand_1, int64_t operand_2, int64_t *p_result);

/**
 * @brief Performs a left bit shift between two uint64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int shift_left_uint64(uint64_t operand_1, uint64_t operand_2,
                      uint64_t *p_result);

/**
 * @brief Performs a right bit shift between two uint64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int shift_right_uint64(uint64_t operand_1, uint64_t operand_2,
                       uint64_t *p_result);

/**
 * @brief Performs bitwise AND between two uint64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int and_uint64(uint64_t operand_1, uint64_t operand_2, uint64_t *p_result);

/**
 * @brief Performs bitwise OR between two uint64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int or_uint64(uint64_t operand_1, uint64_t operand_2, uint64_t *p_result);

/**
 * @brief Performs bitwise XOR between two uint64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int xor_uint64(uint64_t operand_1, uint64_t operand_2, uint64_t *p_result);

/**
 * @brief Performs a left bit rotation between two uint64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int rotate_left_uint64(uint64_t operand_1, uint64_t operand_2,
                       uint64_t *p_result);

/**
 * @brief Performs a right bit rotation between two uint64_t types
 *
 * @param operand_1 The first operand to be passed
 * @param operand_2 The second operand to be passed
 * @param p_result The solution to the equation
 * @return int - Returns 0 on success state, -1 on error state
 */
int rotate_right_uint64(uint64_t operand_1, uint64_t operand_2,
                        uint64_t *p_result);

#endif /* _MATH_FUNCTIONS_H */

/*** end of file ***/
