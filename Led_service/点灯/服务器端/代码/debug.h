#ifndef __PR_DEBUG_H__
#define __PR_DEBUG_H__

/*Comment/uncomment the following line to disable/enable debugging,
 OR define(or NOT) it in Makefile.
*/
//#define NDEBUG

#undef pr_debug					/* undef it, just in case */

#ifndef NDEBUG
#ifdef __KERNEL__
		/* This one if debugging is on, and kernel space */
#define pr_debug(fmt, args...) printk( KERN_ERR fmt, ## args)
#else
#define pr_debug(fmt, args...) fprintf(stderr, fmt, ## args)
#endif

#else
#define pr_debug(fmt, args...)	/* not debugging: nothing */
#endif

#endif
