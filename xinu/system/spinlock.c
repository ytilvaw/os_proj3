void sl_init(sl_lock_t *l);
{
	l = 0;
}

void sl_lock(sl_lock_t *l)
{
	while(testandset(&l, 0, 1) == 1)
	{
		// do nothing
	}
}

void sl_unlock(sl_lock_t *l)
{
	l = 0;
}
