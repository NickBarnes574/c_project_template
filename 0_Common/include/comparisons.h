/** @file comparisons.h
 *
 * @brief Module for comparisons for the datastructures.
 */
#ifndef _COMPARISONS_H
#    define _COMPARISONS_H

// return values all comparison functions should use for standardizing
// behavior
typedef enum comp_rtns
{
    ERROR = -1,
    EQUAL,
    ONE,
    TWO,
    NOT_EQUAL
} comp_rtns_t;

/**
 * @brief A pointer to a user-defined function for comparing data in void
 * sources
 *
 */
typedef comp_rtns_t (*CMP_F)(void *, void *);

/**
 * @brief Fuction to compare integer data inside two void sources
 *
 * @param p_node_one_data pointer to the data from the first source
 * @param p_node_two_data pointer to the data from the second source
 * @return EQUAL (if equal), ONE (if one is larger), TWO (if two is larger),
 * ERROR (on error)
 */
comp_rtns_t int_comp(void * p_node_one_data, void * p_node_two_data);

#endif /* _COMPARISONS_H */

/*** end of file ***/
