
#include <xinu.h>

//#define debug 0

void print_l_arr()
{
	intmask mask;

	
    mask = disable();
	kprintf("lid\t\t available\t\t owner_process\t\t waiting_process \t\t Next_prior\n");
	kprintf("---\t\t ---------\t\t -------------\t\t --------------- \t\t ---------------\n");
	for(int i=0; i<T_LOCKS; i++)
	{
		kprintf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\n", i, l_arr[i].avail, l_arr[i].owner_proc, l_arr[i].wait_proc, l_arr[i].n_prior);
	}
	restore(mask);

}

void l_arr_init()
{
	lock_id = 0;
	
	for(int i =0;i<NPROC;i++)
	{
		d_arr[i] = FALSE;
	}

	for(int i =0;i<T_LOCKS;i++)
	{
		l_arr[i].avail 			= TRUE;
		l_arr[i].owner_proc 	= 500;
		l_arr[i].wait_proc 		= 200;
		l_arr[i].n_prior 		= 0;
	}
}


void search_owner_in_wait_col(bool8 *found, uint32 *temp_lid, pid32 owner)
{
	for(int i=0;i<T_LOCKS;i++)
	{
		if(l_arr[i].wait_proc == owner)
		{
			*found = TRUE;
			*temp_lid = i;
			return;
		}
	}
	*found = FALSE;
}


void al_init(lock_td *l)
{
    *(l->q) = newqueue();

    l->flag = 0;
    l->guard = 0;
	if(lock_id>=T_LOCKS)
	{
		//if(debug){printf("ERROR: Invalid lock id = %d detected\n", lock_id);}
	}
	l->lid = lock_id;
    intmask mask;
    mask = disable();
		//if(debug){printf("lock %d is initialized by process %d\n", l->lid, currpid);}
	restore(mask);
	lock_id++;
}

bool8 al_trylock(lock_td *l)
{
	if(l->flag == 1)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void al_lock(lock_td *l)
{

    struct procent* prptr;
    intmask mask;

    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning

    if(l->flag == 0)
    {
        l->flag = 1;
        mask = disable();
		//if(debug){printf("lock %d is aquired by process %d\n", l->lid, currpid);}
	    restore(mask);
        l->guard = 0;
		if(isbadpid(currpid))
		{
			//if(debug){printf("ERROR: bad pid = %d detected in al_lock\n", currpid);}
		}
		l_arr[l->lid].owner_proc 	= currpid;
		l_arr[l->lid].avail 		= FALSE;

		//if(debug) { print_l_arr(); }
		
        //if(debug){printf("process %d acquired the lock \n", currpid);}
    }
    else
    {
		
        mask = disable();
		//if(debug){printf("process %d is waiting for lock %d and will sleep now\n", currpid, l->lid);}
	    restore(mask);

		/* code for deadlock detection */
		if(isbadpid(currpid))
		{
			//if(debug){printf("ERROR: bad pid = %d detected in al_lock\n", currpid);}
		}

		bool8 	found = FALSE;
		uint32	temp_lid = l->lid;
		
		pid32 owner_pid;
		do
		{
			owner_pid = l_arr[temp_lid].owner_proc;
			search_owner_in_wait_col(&found, &temp_lid, owner_pid);
			d_arr[owner_pid]= TRUE;
			//if(debug){printf("Inside while and owner pid = %d\n", owner_pid);}
		}while(found);
	
		bool8 f_print = FALSE;	
		if(currpid == owner_pid)
		{
			d_arr[owner_pid]= TRUE;
            mask = disable();
			printf("lock_detected=<");
			for(int i=0;i<NPROC;i++)
			{
				if(d_arr[i] == TRUE)
				{
					if(!f_print)
					{
						printf("p%d", i);
						f_print=TRUE;
					}
					else
					{
						printf("-p%d", i);
					}
				}
			}
			printf(">\n");
	        restore(mask);
			// flushing deadlock array
			for(int i =0;i<NPROC;i++)
			{
				d_arr[i] = FALSE;
			}
		}
		else
		{
			l_arr[l->lid].wait_proc = currpid;
			// flushing deadlock array
			for(int i =0;i<NPROC;i++)
			{
				d_arr[i] = FALSE;
			}
		}
			
		//if(debug) { print_l_arr(); }
		/* code for deadlock detection */
        prptr = &proctab[currpid];
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

void al_unlock(lock_td *l)
{
    intmask mask;
    mask = disable();
    //if(debug){printf("Process %d is has called unlock on lock %d\n", currpid, l->lid);}
	restore(mask);
    pid32 temp;
    while(testandset( &l->guard, 1 ) == 1 );    //acquire the guard lock by spinning
    
    if( isempty(*(l->q)) )
    {
        l->flag = 0;
		l_arr[l->lid].avail = TRUE;
		l_arr[l->lid].owner_proc = 500;
        mask = disable();
        //if(debug){printf("process %d relased the lock %d\n", currpid, l->lid);}
	    restore(mask);
    }
    else
    {
        temp = dequeue(*(l->q));
		l_arr[l->lid].owner_proc = temp;
        mask = disable();
        //if(debug){printf("Process %d got the lock %d from %d \n", temp, l->lid, currpid);}
	    restore(mask);
        struct procent* prptr;
        prptr = &proctab[temp];
        prptr->park = FALSE;
        ready(temp);
    }
    
    l->guard = 0;

}
