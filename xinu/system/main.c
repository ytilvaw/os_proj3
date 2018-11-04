/*  main.c  - main */

#include <xinu.h>

process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	uint32 lock, lock_1;
	lock=0;
	lock_1 = 10;
	kprintf("before testandset: lock=%d\n", lock);
	kprintf("before testandset: lock_1=%d\n", lock_1);
	lock_1 = testandset(&lock, 1);
	kprintf("after testandset: lock=%d\n", lock);
	kprintf("after testandset: lock_1=%d\n", lock_1);
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
