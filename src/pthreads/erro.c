// /usr/include/errno.h
//
/* Get the error number constants from the system-specific file.
 *    This file will test __need_Emath and _ERRNO_H.  */
#include <bits/errno.h>

/* Declare the `errno' variable, unless it's defined as a macro by
 * bits/errno.h.  This is the case in GNU, where it is a per-thread
 * variable.  This redeclaration using the macro still works, but it
 * will be a function declaration without a prototype and may trigger
 * a -Wstrict-prototypes warning.  */
#ifndef  errno
extern int errno;
#endif


// /usr/include/bits/errno.h
//
# ifndef __ASSEMBLER__
/* Function to get address of global `errno' variable.  */
extern int *__errno_location (void) __THROW __attribute__ ((__const__));

#  if !defined _LIBC || defined _LIBC_REENTRANT
/* When using threads, errno is a per-thread value.  */
#   define errno (*__errno_location ())
#  endif
# endif /* !__ASSEMBLER__ */
#endif /* _ERRNO_H */

