#include <stdio.h>
#include <stdlib.h>

#include "comparisons.h"
#include "utilities.h"

comp_rtns_t int_comp(void * p_node_one_data, void * p_node_two_data)
{
    comp_rtns_t result   = ERROR;
    int         data_one = 0;
    int         data_two = 0;

    if ((NULL == p_node_one_data) || (NULL == p_node_two_data))
    {
        print_error("NULL argument passed.");
        goto END;
    }

    data_one = *(int *)p_node_one_data;
    data_two = *(int *)p_node_two_data;

    if (data_one == data_two)
    {
        result = EQUAL;
        goto END;
    }

    if (data_one > data_two)
    {
        result = ONE;
        goto END;
    }

    if (data_two > data_one)
    {
        result = TWO;
        goto END;
    }

END:
    return result;
}

/*** end of file ***/
