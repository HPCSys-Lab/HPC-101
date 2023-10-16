
/* Functions for handling conditional variables.  */

/* Initialize condition variable COND using attributes ATTR, or use
   the default values if later is NULL.  */
extern int pthread_cond_init (pthread_cond_t *__restrict __cond,
               __const pthread_condattr_t *__restrict
               __cond_attr) __THROW;

/* Destroy condition variable COND.  */
extern int pthread_cond_destroy (pthread_cond_t *__cond) __THROW;

/* Wake up one thread waiting for condition variable COND.  */
extern int pthread_cond_signal (pthread_cond_t *__cond) __THROW;

/* Wake up all threads waiting for condition variables COND.  */
extern int pthread_cond_broadcast (pthread_cond_t *__cond) __THROW;

/* Wait for condition variable COND to be signaled or broadcast.
   MUTEX is assumed to be locked before.

   This function is a cancellation point and therefore not marked with
   __THROW.  */

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
/* Wait for condition variable COND to be signaled or broadcast until
   ABSTIME.  MUTEX is assumed to be locked before.  ABSTIME is an
   absolute time specification; zero is the beginning of the epoch
   (00:00:00 GMT, January 1, 1970).

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex,
												               const struct timespec *restrict abstime);

/* Functions for handling condition variable attributes.  */

/* Initialize condition variable attribute ATTR.  */
extern int pthread_condattr_init (pthread_condattr_t *__attr) __THROW;

/* Destroy condition variable attribute ATTR.  */
extern int pthread_condattr_destroy (pthread_condattr_t *__attr) __THROW;

/* Get the process-shared flag of the condition variable attribute ATTR.  */
extern int pthread_condattr_getpshared (__const pthread_condattr_t *
                                        __restrict __attr,
                                        int *__restrict __pshared) __THROW;

/* Set the process-shared flag of the condition variable attribute ATTR.  */
extern int pthread_condattr_setpshared (pthread_condattr_t *__attr,
                                        int __pshared) __THROW;

#ifdef __USE_XOPEN2K
/* Get the clock selected for the conditon variable attribute ATTR.  */
extern int pthread_condattr_getclock (__const pthread_condattr_t *
                  __restrict __attr,
                  __clockid_t *__restrict __clock_id)
     __THROW;

/* Set the clock selected for the conditon variable attribute ATTR.  */
extern int pthread_condattr_setclock (pthread_condattr_t *__attr,
                  __clockid_t __clock_id) __THROW;

#endif



int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex,
												               const struct timespec *restrict abstime);
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);

The  pthread_cond_timedwait()  and  pthread_cond_wait() functions shall block on a condition variable. They shall be called with mutex
locked by the calling thread or undefined behavior results.

These functions atomically release mutex and cause the calling thread to block on the condition variable cond; atomically  here  means
"atomically with respect to access by another thread to the mutex and then the condition variable". That is, if another thread is able
to acquire the mutex after the about-to-block  thread  has  released  it,  then  a  subsequent  call  to  pthread_cond_broadcast()  or
pthread_cond_signal() in that thread shall behave as if it were issued after the about-to-block thread has blocked.

Upon successful return, the mutex shall have been locked and shall be owned by the calling thread.

When using condition variables there is always a Boolean predicate involving shared variables associated with each condition wait that
is true if the thread should proceed. Spurious wakeups from the pthread_cond_timedwait() or pthread_cond_wait() functions  may  occur.
Since  the  return from pthread_cond_timedwait() or pthread_cond_wait() does not imply anything about the value of this predicate, the
predicate should be re-evaluated upon such return.

The effect of using more than one mutex for concurrent pthread_cond_timedwait() or pthread_cond_wait() operations on the  same  condi-
tion  variable  is undefined; that is, a condition variable becomes bound to a unique mutex when a thread waits on the condition vari-
able, and this (dynamic) binding shall end when the wait returns.

A condition wait (whether timed or not) is a cancellation  point.  When  the  cancelability  enable  state  of  a  thread  is  set  to
PTHREAD_CANCEL_DEFERRED,  a  side  effect  of  acting  upon  a cancellation request while in a condition wait is that the mutex is (in
effect) re-acquired before calling the first cancellation cleanup handler. The effect is as if the thread were unblocked,  allowed  to
execute  up  to the point of returning from the call to pthread_cond_timedwait() or pthread_cond_wait(), but at that point notices the
cancellation request and instead of returning to the caller of pthread_cond_timedwait() or pthread_cond_wait(), starts the thread can-
cellation activities, which includes calling cancellation cleanup handlers.

A  thread  that  has  been  unblocked  because  it  has  been  canceled  while  blocked  in  a  call  to  pthread_cond_timedwait()  or
pthread_cond_wait() shall not consume any condition signal that may be directed concurrently at the condition variable  if  there  are
other threads blocked on the condition variable.

The  pthread_cond_timedwait()  function  shall  be equivalent to pthread_cond_wait(), except that an error is returned if the absolute
time specified by abstime passes (that is, system time equals or exceeds abstime) before the condition  cond  is  signaled  or  broad-
casted, or if the absolute time specified by abstime has already been passed at the time of the call.

If  the  Clock Selection option is supported, the condition variable shall have a clock attribute which specifies the clock that shall
be used to measure the time specified by the abstime argument.  When such timeouts occur, pthread_cond_timedwait()  shall  nonetheless
release and re-acquire the mutex referenced by mutex. The pthread_cond_timedwait() function is also a cancellation point.

If  a signal is delivered to a thread waiting for a condition variable, upon return from the signal handler the thread resumes waiting
for the condition variable as if it was not interrupted, or it shall return zero due to spurious wakeup.



	struct list {
		pthread_mutex_t lm;
		...
	}

	struct elt {
		key k;
		int busy;
		pthread_cond_t notbusy;
		...
	}

/* Find a list element and reserve it. */
struct elt *
list_find(struct list *lp, key k)
{
	struct elt *ep;

	pthread_mutex_lock(&lp->lm);
	while ((ep = find_elt(l, k) != NULL) && ep->busy)
		pthread_cond_wait(&ep->notbusy, &lp->lm);
	if (ep != NULL)
		ep->busy = 1;
	pthread_mutex_unlock(&lp->lm);
	return(ep);
}

delete_elt(struct list *lp, struct elt *ep)
{
	pthread_mutex_lock(&lp->lm);
	assert(ep->busy);
	... remove ep from list ...
	ep->busy = 0;  /* Paranoid. */
	(A) pthread_cond_broadcast(&ep->notbusy);
	pthread_mutex_unlock(&lp->lm);
	(B) pthread_cond_destroy(&rp->notbusy);
	free(ep);
}

In this example, the condition variable and its list element may be freed (line B) immediately after all threads waiting  for  it  are
awakened (line A), since the mutex and the code ensure that no other thread can touch the element to be deleted.

