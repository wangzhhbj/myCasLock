
#ifndef LTHREAD_MUTEX_H_
#define LTHREAD_MUTEX_H_

#include "rte_atomic.h"


//#include <errno.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

#define POSIX_ERRNO(x)  (x)

#define MAX_MUTEX_NAME_SIZE 32

enum posix_error {
	EPERM = 1,
	EBUSY = 16,
	EINVAL = 22,
	EDEADLK = 35,
};


struct lthread_mutex {
	uint64_t owner;  /* 用pthread_self试下 */
	//rte_atomic64_t	count; /* 加锁线程数量计数*/
	char name[MAX_MUTEX_NAME_SIZE];
} __rte_cache_aligned;


/*
 * Create a mutex
 */
static int
lthread_mutex_init(char *name, struct lthread_mutex *m)
{
	if (m == NULL) {
		return POSIX_ERRNO(EINVAL);
	}

	if (name == NULL)
		strncpy(m->name, "no name", sizeof(m->name));
	else
		strncpy(m->name, name, sizeof(m->name));
	
	m->name[sizeof(m->name)-1] = 0;
	m->owner = 0;

	return 0;
}

/*
 * Destroy a mutex
 */
static int lthread_mutex_destroy(struct lthread_mutex *m)
{
	if (m == NULL) {
		return POSIX_ERRNO(EINVAL);
	}

	if (m->owner == 0) {
		return 0;
	}
	/* can't do its still in use */
	return POSIX_ERRNO(EBUSY);
}

/*
 * Try to obtain a mutex
 */
static int lthread_mutex_lock(struct lthread_mutex *m)
{
	uint64_t tid = pthread_self();

	if (m == NULL) {
		return POSIX_ERRNO(EINVAL);
	}

	/* allow no recursion */
	if (m->owner == tid) {
		return POSIX_ERRNO(EDEADLK);
	}

	for (;;) {
		do {
			if (rte_atomic64_cmpset
			    ((uint64_t *) &m->owner, 0, tid)) {
				/* happy days, we got the lock */
				return 0;
			}
			/* spin due to race with unlock when
			* nothing was blocked
			*/
		}while (m->owner == 0);
		
		sched_yield();
	}
	
	
	return 0;
}

/* try to lock a mutex but don't block */
static int lthread_mutex_trylock(struct lthread_mutex *m)
{
	uint64_t tid = pthread_self();

	if (m == NULL) {
		return POSIX_ERRNO(EINVAL);
	}

	if (m->owner == tid) {
		/* no recursion */
		return POSIX_ERRNO(EDEADLK);
	}

	if (rte_atomic64_cmpset
	    ((uint64_t *) &m->owner, (uint64_t) 0, tid)) {
		/* got the lock */
		return 0;
	}

	return POSIX_ERRNO(EBUSY);
}

/*
 * Unlock a mutex
 */
static int lthread_mutex_unlock(struct lthread_mutex *m)
{
	uint64_t tid = pthread_self();

	if (m == NULL) {
		return POSIX_ERRNO(EINVAL);
	}

	/* fail if its owned */
	if (m->owner != tid || m->owner == 0) {
		return POSIX_ERRNO(EPERM);
	}

	/* release the lock */
	m->owner = 0;
	
	return 0;
}



#endif

