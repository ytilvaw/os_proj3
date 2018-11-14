
#include <xinu.h>
/* performs the summation in parallel using locks */

void init_lock(lock_t *l)
{
    qid16 q;
    q = newqueue();

    //printf("qid = %d\n",q);
    l->flag = 0;
    l->guard = 0;
    *(l->q) = q;
    //queue_init(l->q);
}

void lock(lock_t *l)
{

    struct procent* prptr;
    intmask mask;

    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning

    if(l->flag == 0)
    {
        l->flag = 1;
        l->guard = 0;
        //printf("process %d acquired the lock \n", currpid);
    }
    else
    {
        //queue_add (l->q, currpid);
        enqueue(currpid, *(l->q));
        prptr = &proctab[currpid];
        prptr->park = TRUE;
        l->guard = 0;
        //park(currpid);
        if( prptr->park == TRUE ) //suspend(pid);
        {
            prptr->prstate = PR_WAIT;
            mask = disable();
	        resched();
	        restore(mask);
        }

    }
}

void unlock(lock_t *l)
{
    pid32 temp;
    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning
    
    if( isempty(*(l->q)) )
    {
        l->flag = 0;
        //printf("process %d relased the lock \n", currpid);
    }
    else
    {
        //temp = queue_remove(l->q);
        temp = dequeue(*(l->q));
        //printf("Process %d got the lock from %d \n", temp, currpid);
        struct procent* prptr;
        prptr = &proctab[temp];
        prptr->park = FALSE;
        ready(temp);
    }
    
    l->guard = 0;

}
