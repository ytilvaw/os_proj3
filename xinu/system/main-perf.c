#include <xinu.h>

void compare_p2_p3()
{
	/* getting the execution time for p2 */
	
	uint32 n = 50;
	uint32 array[n];
	uint32 num_threads = 10;
	uint32 sum_ser_arr;
    uint32 sum_naive_arr=0;

	//initializing array
	for(int i=0;i<n;i++)
	{
		array[i] = i*2;
	}
	//sync with lock parallel array
	start_1 = ctr1000;
    sum_naive_arr = sync_parallel_summation_p2(array, n, num_threads);
	uint32 time_1 = stop_1-start_1;

	kprintf("time in p2->%d and sum->%d \n", time_1, sum_naive_arr);

	
	/* getting the execution time for p3 */
	
	start_2 = ctr1000;
    sum_naive_arr = sync_parallel_summation(array, n, num_threads);
	uint32 time_2 = stop_2-start_2;
	kprintf("time in p3->%d and sum->%d \n", time_2, sum_naive_arr);

}
