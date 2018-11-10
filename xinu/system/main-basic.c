/* main-basic.c   */
/* test case 1 to test locks */

#include <xinu.h>


uint32 serial_summation(uint32 *array, uint32 n)
{
    int i = 0;
    uint32 sum = 0;
    for(i=0; i<n; i++)
	{
        sum = sum + *(array+i);
	}

    return sum;
}

uint32 naive_parallel_summation (uint32 *array, uint32 n, uint32 num_threads)
{
}
