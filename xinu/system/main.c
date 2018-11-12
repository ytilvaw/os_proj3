/*  main.c  - main */

#include <xinu.h>

process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	//uint32 lock, lock_1;
	//lock=0;
	//lock_1 = 10;
	//kprintf("before testandset: lock=%d\n", lock);
	//kprintf("before testandset: lock_1=%d\n", lock_1);
	//lock_1 = testandset(&lock, 1);
	//kprintf("after testandset: lock=%d\n", lock);
	//kprintf("after testandset: lock_1=%d\n", lock_1);
	//resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	uint32 n = 50;
	uint32 array[n];
	uint32 num_threads = 10;
	uint32 sum_ser_arr;
    uint32 sum_naive_arr;

	//initializing array
	for(int i=0;i<n;i++)
	{
		array[i] = i*2;
	}
	sum_ser_arr = serial_summation(array, n);	
	kprintf("Sum of array serial -> %d\n", sum_ser_arr);	


    //naive parallel array
    sum_naive_arr = naive_parallel_summation(array, n, num_threads);
	kprintf("Sum of array naive parallel -> %d\n", sum_naive_arr);


    //sync with lock parallel array
    sum_naive_arr = sync_parallel_summation(array, n, num_threads);
	kprintf("Sum of array sync parallel with lock -> %d\n", sum_naive_arr);

	//create_proc_for_deadlock_4p_1d();
	create_proc_for_deadlock_5p_2d();
	return OK;
    
}
