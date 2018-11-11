
#include <xinu.h>

void queue_init(queue_t** q)
{
    //kprintf("inside init function\n");
    //qid16 temp;
    //temp = newqueue();
    //kprintf("newqueue done\n");
    //(*(*q)) = temp;
    //kprintf("initialization done\n");

    **q = newqueue();
}

void queue_add(queue_t* q, pid32 pid)
{
    pid32 ans;
    enqueue(pid, *q);
    //printf("Added the process = %d to the queue\n", pid);
}

bool8 queue_empty(queue_t* q)
{
    return isempty(*q);
}

pid32 queue_remove(queue_t* q)
{
    //printf("removing the process = %d from the queue\n", *q);
    return dequeue(*q);
}

void setpark(pid32 pid)
{
    struct procent* prptr;
    prptr = &proctab[pid];
    prptr->park = TRUE;
}

void park(pid32 pid)
{
    struct procent* prptr;
    prptr = &proctab[pid];
    if( prptr->park == TRUE ) suspend(pid);
}

void unpark( pid32 pid )
{
    struct procent* prptr;
    prptr = &proctab[pid];
    prptr->park = FALSE;
    //resume(pid);  
    ready(pid);
}

void slq_init(lock_tq *l)
{
    qid16 q;
    q = newqueue();

    l->flag = 0;
    l->guard = 0;
    *(l->q) = q;
    //queue_init(l->q);
}

void slq_lock(lock_tq *l)
{
    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning

    if(l->flag == 0)
    {
        l->flag = 1;
        l->guard = 0;
    }
    else
    {
        queue_add (l->q, currpid);
        setpark(currpid);
        l->guard = 0;
        park(currpid);
    }
}

void slq_unlock(lock_tq *l)
{
    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning
    
    if( queue_empty(l->q) )
    {
        l->flag = 0;
    }
    else
    {
        unpark ( queue_remove(l->q) );
    }
    
    l->guard = 0;

}
