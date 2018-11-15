/*  main.c  - main */

#include <xinu.h>

process	main(void)
{

	/* Run the Xinu shell */

	recvclr();

	/* Wait for shell to exit and recreate it */

	/*------------------------ p2 --------------------------*/
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
	//sum_ser_arr = serial_summation(array, n);	
	//kprintf("Sum of array serial -> %d\n", sum_ser_arr);	


    ////naive parallel array
    //sum_naive_arr = naive_parallel_summation(array, n, num_threads);
	//kprintf("Sum of array naive parallel -> %d\n", sum_naive_arr);


    ////sync with lock parallel array
    //sum_naive_arr = sync_parallel_summation(array, n, num_threads);
	//kprintf("Sum of array sync parallel with lock -> %d\n", sum_naive_arr);
	/*------------------------ p2 --------------------------*/

	//create_proc_for_deadlock_5p_2d();
    
    //create_proc_for_deadlock_dr_2p_1d();
	/*------------------------ p4 --------------------------*/
	//create_proc_for_pi_2p();
	//create_proc_for_pi_3p();
	//create_proc_for_pi_4p_3p();

	/*------------------------ p5 --------------------------*/

	//compare_p2_p3();
    //sleepms(5000);
	//compare_p3_p5();

	create_proc_for_transitive();
	/*------------------------ Test cases --------------------------*/

	return OK;
    
}
