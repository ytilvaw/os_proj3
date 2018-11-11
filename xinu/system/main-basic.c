/* main-basic.c   */
/* test case 1 to test locks */

#include <xinu.h>

uint32 sum = 0;
uint32 count = 0;

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
    sum = 0;
    count = 0;
    void indiVidual_sum(uint32*, int);
    for(int i=0; i<num_threads;i++)
    {
    	resume(create((void *)indiVidual_sum, 4096,20, "parallel_summation", 2, array, (i*5) ) );
    	//sleepms(100);
        //kprintf("%d th process has been created, and sum = %d\n", i, sum);
    }	

    while(count < n);
    return sum;
}

void indiVidual_sum(uint32 *array, int start)
{
    int temp;
    for(int i=start; i<(start+5); i++)
    {
        temp = sum;
        temp = temp + *(array+i);
        sleepms(100);
        sum = temp;
        count = count + 1;
    }
    //kprintf( "Sum at the end process = %d, start = %d\n", sum, start );
}


/* performs the summation in parallel using locks */
uint32 sync_parallel_summation  (uint32 *array, uint32 n, uint32 num_threads)
{
    sum = 0;
    count = 0;

    //for spin lock
    //lock_t l;
    //sl_init(&l);
    
    //for lock queue
    lock_tq l;
    slq_init(&l);
    //kprintf("queue initialization success\n");

    //void indiVidual_sum_sync(uint32*, int, lock_t*);     // for spin lock
    void indiVidual_sum_sync(uint32*, int, lock_tq*);      // for lock queue 
    
    for(int i=0; i<num_threads;i++)
    {
    	resume(create((void *)indiVidual_sum_sync, 4096,20, "parallel_summation", 3, array, (i*5), &l ) );
    	//sleepms(200);
        //kprintf("%d th process has been created, and sum = %d\n", i, sum);
    }	
    while(count < n);
    return sum;

}


//void indiVidual_sum_sync(uint32 *array, int start, lock_t* l) // for spin lock
void indiVidual_sum_sync(uint32 *array, int start, lock_tq* l)  // for lock queue
{
    int temp;
    struct procent* prptr;
    //kprintf("process with start = %d acquired lock\n",start);
    for(int i=start; i<(start+5); i++)
    {
        //sl_lock(l);             // for spin lock
        slq_lock(l);              // for lock queue 
        temp = sum;
        temp = temp + *(array+i);
        sleepms(100);
        sum = temp;
        count = count + 1;
        //printf("currpid = %d\n", currpid);
        prptr = &proctab[currpid];
        //printf("state = %d\n", prptr->prstate);
        //sl_unlock(l);          // for spin lock
        slq_unlock(l);           // for lock queue 
    }
    //kprintf("process with start = %d released lock\n",start);
    //kprintf("sum = %d\n",sum);
    //kprintf( "Sum at the end process = %d, start = %d\n", sum, start );
}

