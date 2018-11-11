
typedef struct __lock_t { 
    int flag; 
} lock_t;

//for part3
typedef qid16 queue_t;

typedef struct __lock_tq {
    int flag;
    int guard;
    queue_t* q;
} lock_tq;

void sl_init(lock_t *l);
void sl_lock(lock_t *l);
void sl_unlock(lock_t *l);


void queue_init(queue_t** q);
void queue_add(queue_t* q, pid32 pid);
bool8 queue_empty(queue_t* q);
pid32 queue_remove(queue_t* q);
void setpark(pid32 pid);
void park(pid32 pid);
void unpark( pid32 pid );
void slq_init(lock_tq *l);
void slq_lock(lock_tq *l);
void slq_unlock(lock_tq *l);

