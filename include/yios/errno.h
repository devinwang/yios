/* errno is not a global variable, because that would make using it
   non-reentrant.  Instead, its address is returned by the function
   __errno.  */

#ifndef _SYS_ERRNO_H_
#define _SYS_ERRNO_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/errno_values.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CFG_YIOS_KERNEL
#   include <sys/reent.h>

#   ifndef _REENT_ONLY
#       define errno (*__errno())
        extern int *__errno _PARAMS ((void));
#   endif

    /* Please don't use these variables directly.
        Use strerror instead. */
    extern __IMPORT _CONST char * _CONST _sys_errlist[];
    extern __IMPORT int _sys_nerr;
#   ifdef __CYGWIN__
    extern __IMPORT const char * const sys_errlist[];
    extern __IMPORT int sys_nerr;
#   endif

#   define __errno_r(ptr) ((ptr)->_errno)

#   define NERR(x)      (x)

#else /* CFG_YIOS_KERNEL */


#endif /* CFG_YIOS_KERNEL */


#define __ELASTERROR    2000	/* Users can add values starting here */

/* custom system error number. */
#if ARCH_SIZE > 8
#define __ESYSLASTERROR		__ELASTERROR
#else
#define __ESYSLASTERROR		140
#endif

#define E_OK			0

#define SYSERR(__errx__)       (-(__errx__))


#ifdef CFG_YIOS_KERNEL
#define ETHPRNG			(__ESYSLASTERROR+1)	/* thread priority out of range */
#define ETHEXIST		(__ESYSLASTERROR+2)	/* thread priority exists*/
#define	EINTRNG         (__ESYSLASTERROR+3)	/* Interrupter Channel number out of range */
#define EPARAM          (__ESYSLASTERROR+4) /* param error. `*/
#define ENOINTSHARE     (__ESYSLASTERROR+5) /* can't share interrupter channel. */
#define ENOINTHD        (__ESYSLASTERROR+6) /* no interrupt handle. */
#define ENOSEM          (__ESYSLASTERROR+7) /* can't get semaphore. */
#endif /* CFG_YIOS_KERNEL */

#ifdef __cplusplus
}
#endif
#endif /* _SYS_ERRNO_H */


