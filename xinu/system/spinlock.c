typedef struct __lock_t { int flag; } lock_t;

void sl_init(lock_t *l);
{
	l->flag = 0;
}

void sl_lock(lock_t *l)
{
	while(testandset(&l->flag, 1) == 1)
	{
		// do nothing
	}
}

void sl_unlock(sl_lock_t *l)
{
	l->flag = 0;
}
