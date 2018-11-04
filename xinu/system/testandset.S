/* testandset.S - testandset (for x86) */

		.text
		.globl	testandset

/*------------------------------------------------------------------------
 * testandset -  to exchange the content of two registers atomically
 *------------------------------------------------------------------------
 */
testandset:

		pushl	%ebp		/* Push ebp onto stack		*/
		movl	%esp,%ebp	/* Record current SP in ebp	*/
		pushfl			/* Push flags onto the stack	*/
		pushl	%ebx		/* Push ebp onto stack		*/


		movl	8(%ebp),%ebx	/* Get the address of lock variable	*/
		movl	12(%ebp),%eax	/* Get the value of "new_value"	*/

		xchg	(%ebx), %eax	/* to do atomic exchange between content of eax and ebx*/
		
		pop		%ebx			/* restore ebx */
	
		/* By default, in x86 value of eax is used as a return value */

		popfl			/* Restore interrupt mask	*/
		pop		%ebp	/* Restore %ebp	*/
		ret			/* Return to new process	*/
