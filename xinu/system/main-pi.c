/* main-deadlock*/
/* test case 3-part 1 to test locks */

#include <xinu.h>


/*-----------------------------------------------------------------------------*/
void printA_pi(pi_lock_t *l0)
{
	intmask mask;
	pi_lock(l0);
	sleepms(1000);
    //mask = disable();
	//kprintf("This print is with lock l0 in printA_pi\n");
	//restore(mask);
	
	pi_unlock(l0);
}

void printB_pi(pi_lock_t *l0)
{
	sleepms(500);
	intmask mask;
	pi_lock(l0);
    //mask = disable();
	//kprintf("This print is with lock l0 in printB_pi\n");
	//restore(mask);
	
	pi_unlock(l0);
}

void create_proc_for_pi_2p()
{
	pi_lock_t l0;

	pi_init(&l0);

	resume(create((void *)printA_pi, 4096,20, "func_printA_pi", 1, &l0) );
	resume(create((void *)printB_pi, 4096,40, "func_printB_pi", 1, &l0) );
}

/*-----------------------------------------------------------------------------*/

void print_p1_pi(pi_lock_t *l0, pi_lock_t *l1)
{
	intmask mask;
	pi_lock(l0);
	pi_lock(l1);

	sleepms(1000);

	pi_unlock(l1);
	pi_unlock(l0);
}

void print_p2_pi(pi_lock_t *l0)
{
	sleepms(300);
	intmask mask;
	pi_lock(l0);
	sleepms(200);
	pi_unlock(l0);
}


void print_p3_pi(pi_lock_t *l1)
{
	sleepms(500);
	intmask mask;
	pi_lock(l1);

	sleepms(200);
	pi_unlock(l1);
}

void create_proc_for_pi_3p()
{
	pi_lock_t l1;
	pi_lock_t l2;

	pi_init(&l1);
	pi_init(&l2);

	resume(create((void *)print_p1_pi, 4096,10, "func_print_p1_pi", 2, &l1, &l2) );
	resume(create((void *)print_p2_pi, 4096,20, "func_print_p2_pi", 1, &l1) );
	resume(create((void *)print_p3_pi, 4096,30, "func_print_p3_pi", 1, &l2) );
}


/*-----------------------------------------------------------------------------*/

void print_p1_pi_4p(pi_lock_t *l0, pi_lock_t *l1, pi_lock_t *l2)
{
	intmask mask;
	pi_lock(l0);
	pi_lock(l1);
	pi_lock(l2);

	sleepms(1000);

	pi_unlock(l0);
	pi_unlock(l1);
	pi_unlock(l2);
}

void print_p2_pi_4p(pi_lock_t *l0)
{
	sleepms(300);
	intmask mask;
	pi_lock(l0);
	sleepms(200);
	pi_unlock(l0);
}


void print_p3_pi_4p(pi_lock_t *l1)
{
	sleepms(500);
	intmask mask;
	pi_lock(l1);

	sleepms(200);
	pi_unlock(l1);
}

void print_p4_pi_4p(pi_lock_t *l2)
{
	sleepms(700);
	intmask mask;
	pi_lock(l2);

	sleepms(200);
	pi_unlock(l2);
}


void create_proc_for_pi_4p_3p()
{
	pi_lock_t l1;
	pi_lock_t l2;
	pi_lock_t l3;

	pi_init(&l1);
	pi_init(&l2);
	pi_init(&l3);

	resume(create((void *)print_p1_pi_4p, 4096,10, "func_print_p1_pi_4p", 3, &l1, &l2, &l3) );
	resume(create((void *)print_p2_pi_4p, 4096,20, "func_print_p2_pi_4p", 1, &l1) );
	resume(create((void *)print_p3_pi_4p, 4096,30, "func_print_p3_pi_4p", 1, &l2) );
	resume(create((void *)print_p4_pi_4p, 4096,40, "func_print_p3_pi_4p", 1, &l3) );
}


/*-----------------------------------------------------------------------------*/

void run_p5(pi_lock_t *l0)
{
	sleepms(500);
	intmask mask;
	pi_lock(l0);
    //mask = disable();
	//kprintf("This print is with lock l0 in run_p5\n");
	//restore(mask);
	
	pi_unlock(l0);
	uint32 time_5 = (ctr1000 - start_5);
	
    //mask = disable();
	kprintf("p5->%d\n", time_5);
	//restore(mask);
}


void run_p3(lock_t *l0)
{
	sleepms(500);
	intmask mask;
	lock(l0);
    //mask = disable();
	//kprintf("This print is with lock l0 in run_p3\n");
	//restore(mask);
	
	unlock(l0);
	uint32 time_3 = (ctr1000 - start_3);
	
    mask = disable();
	kprintf("p3->%d\n", time_3);
	restore(mask);
}


void compare_p3_p5()
{
	pi_lock_t l0;

	pi_init(&l0);

	resume(create((void *)printA_pi, 4096,20, "func_printA_pi", 1, &l0) );
	start_5 = ctr1000;
	resume(create((void *)run_p5, 4096,40, "func_run_p5", 1, &l0) );

	sleepms(1000);

	lock_t l1;

	init_lock(&l1);
	resume(create((void *)printA_pi, 4096,20, "func_printA_pi", 1, &l1) );
	start_3 = ctr1000;
	resume(create((void *)run_p3, 4096,40, "func_run_p5", 1, &l1) );

}

