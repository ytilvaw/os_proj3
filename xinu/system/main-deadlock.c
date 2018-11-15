/* main-deadlock*/
/* test case 3-part 1 to test locks */

#include <xinu.h>


void printA(al_lock_t *l0, al_lock_t *l1)
{
	intmask mask;
	al_lock(l0);
    mask = disable();
	printf("This print is with lock l0 in printA\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l1);
    mask = disable();
	printf("This print is with lock l1 in printA\n");
	restore(mask);

	al_unlock(l0);
	al_unlock(l1);
}

void printB(al_lock_t *l1, al_lock_t *l2)
{

	intmask mask;
	al_lock(l1);
    mask = disable();
	printf("This print is with lock l1 in printB\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l2);
    mask = disable();
	printf("This print is with lock l2 in printB\n");
	restore(mask);

	al_unlock(l1);
	al_unlock(l2);
}

void printC(al_lock_t *l2, al_lock_t *l3)
{
	intmask mask;
	al_lock(l2);
    mask = disable();
	printf("This print is with lock l2 in printC\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l3);
    mask = disable();
	printf("This print is with lock l3 in printC\n");
	restore(mask);

	al_unlock(l2);
	al_unlock(l3);
}

void printD(al_lock_t *l3, al_lock_t *l0)
{

	intmask mask;
	al_lock(l3);
    mask = disable();
	printf("This print is with lock l3 in printD\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l0);
    mask = disable();
	printf("This print is with lock l0 in printD\n");
	restore(mask);

	al_unlock(l0);
	al_unlock(l3);
}


void create_proc_for_deadlock_4p_1d()
{
	al_lock_t l0;
	al_lock_t l1;
	al_lock_t l2;
	al_lock_t l3;

	
	al_init(&l0);
	al_init(&l1);
	al_init(&l2);
	al_init(&l3);

	resume(create((void *)printA, 4096,20, "func_printA", 2, &l0, &l1) );
	//sleepms(2000);
	resume(create((void *)printB, 4096,20, "func_printB", 2, &l1, &l2) );
	//sleepms(2000);
	resume(create((void *)printC, 4096,20, "func_printC", 2, &l2, &l3) );
	//sleepms(2000);
	resume(create((void *)printD, 4096,20, "func_printD", 2, &l3, &l0) );
}







/*  for two deadlock using 5 processes  */


 
void printA_d2(al_lock_t *l0, al_lock_t *l1)
{
	intmask mask;
	al_lock(l0);
    mask = disable();
	printf("This print is with lock l0 in printA_d2\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l1);
    mask = disable();
	printf("This print is with lock l1 in printA_d2\n");
	restore(mask);

	al_unlock(l0);
	al_unlock(l1);
}

void printB_d2(al_lock_t *l1, al_lock_t *l2)
{

	intmask mask;
	al_lock(l1);
    mask = disable();
	printf("This print is with lock l1 in printB_d2\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l2);
    mask = disable();
	printf("This print is with lock l2 in printB_d2\n");
	restore(mask);

	al_unlock(l1);
	al_unlock(l2);
}

void printC_d2(al_lock_t *l2, al_lock_t *l0)
{
	intmask mask;
	al_lock(l2);
    mask = disable();
	printf("This print is with lock l2 in printC_d2\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l0);
    mask = disable();
	printf("This print is with lock l0 in printC_d2\n");
	restore(mask);

	al_unlock(l2);
	al_unlock(l0);
}

void printD_d2(al_lock_t *l3, al_lock_t *l4)
{

	intmask mask;
	al_lock(l3);
    mask = disable();
	printf("This print is with lock l3 in printD_d2\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l4);
    mask = disable();
	printf("This print is with lock l4 in printD_d2\n");
	restore(mask);

	al_unlock(l3);
	al_unlock(l4);
}

void printE_d2(al_lock_t *l4, al_lock_t *l3)
{

	intmask mask;
	al_lock(l4);
    mask = disable();
	printf("This print is with lock l4 in printE_d2\n");
	restore(mask);
	
	sleepms(1000);

	al_lock(l3);
    mask = disable();
	printf("This print is with lock l3 in printE_d2\n");
	restore(mask);

	al_unlock(l4);
	al_unlock(l3);
}

void create_proc_for_deadlock_5p_2d()
{
	al_lock_t l0;
	al_lock_t l1;
	al_lock_t l2;
	al_lock_t l3;
	al_lock_t l4;

	
	al_init(&l0);
	al_init(&l1);
	al_init(&l2);
	al_init(&l3);
	al_init(&l4);

	resume(create((void *)printA_d2, 4096,20, "func_printA_d2", 2, &l0, &l1) );
	//sleepms(2000);
	resume(create((void *)printB_d2, 4096,20, "func_printB_d2", 2, &l1, &l2) );
	//sleepms(2000);
	resume(create((void *)printC_d2, 4096,20, "func_printC_d2", 2, &l2, &l0) );
	//sleepms(2000);
	resume(create((void *)printD_d2, 4096,20, "func_printD_d2", 2, &l3, &l4) );
	//sleepms(2000);
	resume(create((void *)printE_d2, 4096,20, "func_printE_d2", 2, &l4, &l3) );
}


 
 
 
 
 
 
void printA_dr(al_lock_t *l0, al_lock_t *l1)
{
	intmask mask;
	al_lock(l0);
	//bool8 success = al_trylock(l0);
    mask = disable();
	printf("This print is with lock l0 in printA_dr\n");
	restore(mask);
	
	sleepms(1000);

	//al_lock(l1);
	bool8 success;
l0: al_trylock(l1);
    if(!success)
    {
        al_unlock(l0);
        //goto l0;
    }
    mask = disable();
	printf("This print is with lock l1 in printA_dr\n");
	restore(mask);

	al_unlock(l0);
	al_unlock(l1);
}

void printB_dr(al_lock_t *l1, al_lock_t *l0)
{

	intmask mask;
	al_lock(l1);
	//bool8 success = al_trylock(l1);
    mask = disable();
	printf("This print is with lock l1 in printB_dr\n");
	restore(mask);
	
	sleepms(1000);

	//al_lock(l0);	
    bool8 success;
l1: success = al_trylock(l0);
    if(!success)
    {
        al_unlock(l1);
        //goto l1;
    }

    mask = disable();
	printf("This print is with lock l0 in printB_dr\n");
	restore(mask);

	al_unlock(l1);
	al_unlock(l0);
}


void create_proc_for_deadlock_dr_2p_1d()
{
	al_lock_t l0;
	al_lock_t l1;

	
	al_init(&l0);
	al_init(&l1);

	resume(create((void *)printA_dr, 4096,20, "func_printA_dr", 2, &l0, &l1) );
	//sleepms(2000);
	resume(create((void *)printB_dr, 4096,20, "func_printB_dr", 2, &l1, &l0) );
	//sleepms(2000);
}




