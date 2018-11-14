
#include <xinu.h>

#define debug 0 

pri16 find_n_max(pid32 owner)
{
	pri16 p_max=0;
	for(int i=0;i<T_LOCKS;i++)
	{
		if(l_arr[i].owner_proc == owner)
		{
			if(l_arr[i].n_prior > p_max)
			{	
				p_max = l_arr[i].n_prior;
			}
		}
	}
	return p_max;
}


void pi_init(pi_lock_t *l)
{
    *(l->q) = newqueue();

    l->flag = 0;
    l->guard = 0;
	if(lock_id>=T_LOCKS)
	{
		if(debug){kprintf("ERROR: Invalid lock id = %d detected\n", lock_id);}
	}
	l->lid = lock_id;
    intmask mask;
    mask = disable();
	if(debug){kprintf("lock %d is initialized by process %d\n", l->lid, currpid);}
	restore(mask);
	lock_id++;
}

void pi_lock(pi_lock_t *l)
{

    struct procent* prptr;
    intmask mask;

    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning

    if(l->flag == 0)
    {
        l->flag = 1;
        mask = disable();
		if(debug){kprintf("lock %d is aquired by process %d\n", l->lid, currpid);}
	    restore(mask);
        l->guard = 0;
		if(isbadpid(currpid))
		{
			if(debug){kprintf("ERROR: bad pid = %d detected in pi_lock\n", currpid);}
		}
		l_arr[l->lid].owner_proc 	= currpid;
		l_arr[l->lid].avail 		= FALSE;

		if(debug) { print_l_arr(); }
    }
    else
    {
	
        prptr = &proctab[currpid];
		/*------------------------ code for priority inversion ------------------------*/
	
        mask = disable();
		if(debug){kprintf("lid:%d \tl_arr[l->lid].owner_proc:%d\t proctab[l_arr[l->lid].owner_proc].prprio:%d\t proctab[l_arr[l->lid].owner_proc].o_prprio:%d\t prptr->prprio:%d \tl_arr[l->lid].n_prior:%d\n", l->lid, l_arr[l->lid].owner_proc, proctab[l_arr[l->lid].owner_proc].prprio, proctab[l_arr[l->lid].owner_proc].o_prprio, prptr->prprio, l_arr[l->lid].n_prior);}
	    restore(mask);

		if((proctab[l_arr[l->lid].owner_proc].prprio < prptr->prprio) && (proctab[l_arr[l->lid].owner_proc].o_prprio < prptr->prprio) && (l_arr[l->lid].n_prior < prptr->prprio))
		{
			l_arr[l->lid].n_prior = prptr->prprio;
			pri16 old_prio = proctab[l_arr[l->lid].owner_proc].prprio;
			proctab[l_arr[l->lid].owner_proc].prprio = prptr->prprio;
            mask = disable();
			kprintf("priority_change=P%d::%d-%d\n",l_arr[l->lid].owner_proc, old_prio, proctab[l_arr[l->lid].owner_proc].prprio);
	        restore(mask);
		}
		if(debug) { print_l_arr(); }
		/*------------------------ code for priority inversion ------------------------*/
	
        mask = disable();
		if(debug){kprintf("process %d is waiting for lock %d and will sleep now\n", currpid, l->lid);}
	    restore(mask);

		if(isbadpid(currpid))
		{
			kprintf("ERROR: bad pid = %d detected in pi_lock\n", currpid);
		}

        enqueue(currpid, *(l->q));
        prptr->park = TRUE;
        l->guard = 0;
        if( prptr->park == TRUE ) 
        {
            prptr->prstate = PR_WAIT;
            mask = disable();
	        resched();
	        restore(mask);
        }

    }
}

void pi_unlock(pi_lock_t *l)
{
    intmask mask;
    mask = disable();
    if(debug){kprintf("Process %d is has called unlock on lock %d\n", currpid, l->lid);}
	restore(mask);
    pid32 temp;
    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning
    
	/*------------------------ code for priority inversion ------------------------*/
	
    struct procent* prptr;
    prptr = &proctab[currpid];
	l_arr[l->lid].n_prior = 0;
		

	if(prptr->prprio < prptr->o_prprio)
	{
		kprintf("ERROR: process priority was decresed than its default priority\n");
	}

	if(prptr->prprio > prptr->o_prprio)
	{
		pri16 max_n_prior = find_n_max(l_arr[l->lid].owner_proc);
		if(debug==1){kprintf("max_n_prior is %d and prptr->o_prprio is %d\n", max_n_prior, prptr->o_prprio);}
		if(max_n_prior > prptr->o_prprio)
		{
			pri16 old_prio = prptr->prprio;
			prptr->prprio = max_n_prior;
			if(old_prio != prptr->prprio)
			{
        		mask = disable();
				kprintf("priority_change=P%d::%d-%d\n",l_arr[l->lid].owner_proc, old_prio, prptr->prprio);
	    		restore(mask);
			}
		}
		else
		{
			pri16 old_prio = prptr->prprio;
			prptr->prprio = prptr->o_prprio;
        	mask = disable();
			kprintf("priority_change=P%d::%d-%d\n",l_arr[l->lid].owner_proc, old_prio, prptr->prprio);
	    	restore(mask);
		}
	}

	/*------------------------ code for priority inversion ------------------------*/

    if( isempty(*(l->q)) )
    {
        l->flag = 0;
		l_arr[l->lid].avail = TRUE;
		l_arr[l->lid].owner_proc = 500;
        mask = disable();
        if(debug){kprintf("process %d relased the lock %d\n", currpid, l->lid);}
	    restore(mask);
    }
    else
    {
        temp = dequeue(*(l->q));
		l_arr[l->lid].owner_proc = temp;
        mask = disable();
        if(debug){kprintf("Process %d got the lock %d from %d \n", temp, l->lid, currpid);}
	    restore(mask);
        struct procent* prptr_temp;
        prptr_temp = &proctab[temp];
        prptr_temp->park = FALSE;
        ready(temp);
    }
    
    l->guard = 0;

}
