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
	//creating 10 processes to run naive_parallel_summation func
	//for(int i=0; i<10;i++)
	//{
	//	resume(create((void *)naive_parallel_summation, 4096,20, "parallel_summation", 3, array, n, num_threads));
	//	kprintf("%d th process has been created\n", i);
	//}	
	//sum_par_arr = serial_summation(array, n, 10);
	
	//recvclr();
	//resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	//while (TRUE) {
	//	receive();
	//	sleepms(200);
	//	kprintf("\n\nMain process recreating shell\n\n");
	//	resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	//}
	return OK;
    
}
