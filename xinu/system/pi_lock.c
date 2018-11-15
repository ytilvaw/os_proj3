
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
    qid16 temp = newqueue();
    (l->q) = temp;

    l->flag = 0;
    l->guard = 0;
	if(lock_id>=T_LOCKS)
	{
		if(debug){kprintf("ERROR: Invalid lock id = %d detected\n", lock_id);}
	}
	l->lid = lock_id;
    intmask mask;
    mask = disable();
    if(debug){kprintf("queue id of lock %d returned is %d and assigned as %d\n", l->lid, temp, (l->q));}
	if(debug){kprintf("lock %d is initialized by process %d\n", l->lid, currpid);}
	restore(mask);
	lock_id++;
}

void pi_lock(pi_lock_t *l)
{

    struct procent* prptr;
    intmask mask;

    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning
	mask = disable();
    if(debug){kprintf("Process %d has aquired the guard in lock\n", currpid);}
	restore(mask);

    if(l->flag == 0)
    {
        l->flag = 1;
        mask = disable();
		if(debug){kprintf("lock %d is aquired by process %d\n", l->lid, currpid);}
	    restore(mask);
        l->guard = 0;

    	mask = disable();
    	if(debug){kprintf("process %d relased the guard in lock if %d\n", currpid);}
		restore(mask);

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
		if(debug){kprintf("currpid:%d\tlid:%d \tl_arr[l->lid].owner_proc:%d\t proctab[l_arr[l->lid].owner_proc].prprio:%d\t proctab[l_arr[l->lid].owner_proc].o_prprio:%d\t prptr->prprio:%d \tl_arr[l->lid].n_prior:%d\n", currpid, l->lid, l_arr[l->lid].owner_proc, proctab[l_arr[l->lid].owner_proc].prprio, proctab[l_arr[l->lid].owner_proc].o_prprio, prptr->prprio, l_arr[l->lid].n_prior);}
	    restore(mask);

		if((proctab[l_arr[l->lid].owner_proc].prprio < prptr->prprio) && (proctab[l_arr[l->lid].owner_proc].o_prprio < prptr->prprio) && (l_arr[l->lid].n_prior < prptr->prprio))
		{
			l_arr[l->lid].n_prior = prptr->prprio;
			uint32 curr_lid = l->lid;
			do
			{
				pri16 old_prio = proctab[l_arr[curr_lid].owner_proc].prprio;
				proctab[l_arr[curr_lid].owner_proc].prprio = prptr->prprio;
            	mask = disable();
				kprintf("priority_change=P%d::%d-%d\n",l_arr[curr_lid].owner_proc, old_prio, proctab[l_arr[curr_lid].owner_proc].prprio);
	        	restore(mask);

				if(proctab[l_arr[curr_lid].owner_proc].prstate == PR_WAIT)
				{
					curr_lid = proctab[l_arr[curr_lid].owner_proc].wait_lock; 
				}
				else
				{
					curr_lid = 200;
				}
				
			}while(curr_lid != 200);
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

        insert( currpid, (l->q), proctab[currpid].prprio );
        mask = disable();
		if(debug){kprintf("process %d has emtered sleep queue %d of lock %d\n", currpid, (l->q), l->lid);}
	    restore(mask);
        //enqueue(currpid, (l->q));
        prptr->park = TRUE;
        l->guard = 0;

    	mask = disable();
    	if(debug){kprintf("process %d relased the guard in lock else %d\n", currpid);}
		restore(mask);

        if( prptr->park == TRUE ) 
        {
            prptr->prstate = PR_WAIT;
            prptr->wait_lock = l->lid;
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
		
	mask = disable();
    if(debug){kprintf("Process %d has aquired the guard in unlock\n", currpid);}
	restore(mask);


	if(prptr->prprio < prptr->o_prprio)
	{
		kprintf("ERROR: process priority was decresed than its default priority\n");
	}

	if(prptr->prprio > prptr->o_prprio)
	{
		pri16 max_n_prior = find_n_max(l_arr[l->lid].owner_proc);
        mask = disable();
		if(debug==1){kprintf("max_n_prior is %d and prptr->o_prprio is %d\n", max_n_prior, prptr->o_prprio);}
	    restore(mask);
		
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

    if( isempty((l->q)) )
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
        temp = dequeue((l->q));
		l_arr[l->lid].owner_proc = temp;
		proctab[temp].wait_lock = 200;
        mask = disable();
        if(debug){kprintf("Process %d got the lock %d from %d \n", temp, l->lid, currpid);}
		if(debug){kprintf("process %d has been removed from  sleep queue %d of lock %d\n", temp, (l->q), l->lid);}
	    restore(mask);
        struct procent* prptr_temp;
        prptr_temp = &proctab[temp];
        prptr_temp->park = FALSE;
        ready(temp);
    }
    
    l->guard = 0;

    mask = disable();
    if(debug){kprintf("process %d relased the guard in unlock %d\n", currpid);}
	restore(mask);
}
