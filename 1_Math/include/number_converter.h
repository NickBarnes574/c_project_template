/**
 * @file number_converter.h
 *
 * @brief Module for converting strings to int32_t and uint32_t data types
 */

#ifndef _NUMBER_CONVERTER_H
#define _NUMBER_CONVERTER_H

#include <stdint.h>

typedef union number
{
    int32_t  signed_num;
    uint32_t unsigned_num;
} number_t;

// Covers 4.1.9: proper declaration for created functions
/**
 * @brief Convert a string to an int32_t
 *
 * @param p_input A string of numbers when converted, are within the bounds of
 * 'int32_t'
 * @param p_output A 'number_t union 'int32_t' representation of the input
 * string
 * @return int - Returns 0 on success state, -1 on error state
 */
int str_to_int32(const char * p_input, number_t * p_output);

/**
 * @brief
 *
 * @param p_input A string of numbers when converted, are within the bounds of
 * 'uint32_t'
 * @param p_output A 'number_t' union 'uint32_t' representation of the input
 * string
 * @return int - Returns 0 on success state, -1 on error state
 */
int str_to_uint32(const char * p_input, number_t * p_output);

#endif /* _NUMBER_CONVERTER_H */

/*** end of file ***/
