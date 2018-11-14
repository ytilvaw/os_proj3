

/*-------------------for p2----------------------------------*/
uint32 start_1;
uint32 stop_1;
typedef struct __sl_lock_t { 
    int flag; 
} sl_lock_t;

void sl_init(sl_lock_t *l);
void sl_lock(sl_lock_t *l);
void sl_unlock(sl_lock_t *l);
/*-------------------for p2----------------------------------*/



/*-------------------for p3----------------------------------*/
typedef qid16 queue_t;

uint32 start_2;
uint32 start_3;
uint32 start_5;
uint32 stop_2;

typedef struct __lock_t {
    int flag;
    int guard;
    queue_t* q;
} lock_t;


void init_lock(lock_t *l);
void lock(lock_t *l);
void unlock(lock_t *l);
/*-------------------for p3----------------------------------*/

/*-------------------for p4----------------------------------*/

uint32 lock_id;

typedef struct __al_lock_t {
    int 		flag;
    int 		guard;
    queue_t* 	q;
	uint32		lid;
} al_lock_t;



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
void 	al_init(al_lock_t *l);
void 	al_lock(al_lock_t *l);
void 	al_unlock(al_lock_t *l);
bool8 	al_trylock(al_lock_t *l);
/*-------------------for p4----------------------------------*/



/*-------------------for p5----------------------------------*/

typedef struct __pi_lock_t {
    int 		flag;
    int 		guard;
    queue_t* 	q;
	uint32		lid;
} pi_lock_t;

pri16 	find_n_max(pid32 owner);
void 	pi_init(pi_lock_t *l);
void 	pi_lock(pi_lock_t *l);
void 	pi_unlock(pi_lock_t *l);
/*-------------------for p5----------------------------------*/

