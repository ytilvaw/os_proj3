
typedef struct __lock_t { int flag; } lock_t;

void sl_init(lock_t *l);
void sl_lock(lock_t *l);
void sl_unlock(lock_t *l);
