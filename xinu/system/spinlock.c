#include <xinu.h>

void sl_init(sl_lock_t *l)
{
	l->flag = 0;
}

void sl_lock(sl_lock_t *l)
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
