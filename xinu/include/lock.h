

/*-------------------for p2----------------------------------*/
typedef struct __lock_t { 
    int flag; 
} lock_t;

void sl_init(lock_t *l);
void sl_lock(lock_t *l);
void sl_unlock(lock_t *l);
/*-------------------for p2----------------------------------*/



/*-------------------for p3----------------------------------*/
typedef qid16 queue_t;

typedef struct __lock_tq {
    int flag;
    int guard;
    queue_t* q;
} lock_tq;


void slq_init(lock_tq *l);
void slq_lock(lock_tq *l);
void slq_unlock(lock_tq *l);
/*-------------------for p3----------------------------------*/

/*-------------------for p4----------------------------------*/

uint32 lock_id;

typedef struct __lock_td {
    int 		flag;
    int 		guard;
    queue_t* 	q;
	uint32		lid;
} lock_td;



struct l_arr_entry
{
	bool8 	avail;
	pid32 	owner_proc;
	pid32	wait_proc;
	pri16	n_prior;
};

struct l_arr_entry l_arr[T_LOCKS];
bool8 d_arr[NPROC];

void	print_l_arr();
void 	search_owner_in_wait_col(bool8 *found, uint32 *temp_lid, pid32 owner);
void 	al_init(lock_td *l);
void 	al_lock(lock_td *l);
void 	al_unlock(lock_td *l);
bool8 	al_trylock(lock_td *l);
/*-------------------for p4----------------------------------*/



/*-------------------for p5----------------------------------*/

typedef struct __lock_tpi {
    int 		flag;
    int 		guard;
    queue_t* 	q;
	uint32		lid;
} lock_tpi;

pri16 	find_n_max(pid32 owner);
void 	pi_init(lock_tpi *l);
void 	pi_lock(lock_tpi *l);
void 	pi_unlock(lock_tpi *l);
/*-------------------for p5----------------------------------*/

