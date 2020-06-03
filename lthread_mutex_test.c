
#include <pthread.h>
#include <stdio.h>

#define NLOOP 5000000

#include "lthread_mutex.h"

struct lthread_mutex mutex_lock_st;


static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
static int counter = 0;

void *doit(void *);


int main()
{
    pthread_t tidA, tidB, tidC, tidD, tidE, tidF;
	
	lthread_mutex_init("lthread_mutex_locktest", &mutex_lock_st);
	
    pthread_create(&tidA, NULL, doit, NULL);
    pthread_create(&tidB, NULL, doit, NULL);
    pthread_create(&tidC, NULL, doit, NULL);
    pthread_create(&tidD, NULL, doit, NULL);
    pthread_create(&tidE, NULL, doit, NULL);
    pthread_create(&tidF, NULL, doit, NULL);

    /*wait for both threads to terminate*/
    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);
    pthread_join(tidC, NULL);
    pthread_join(tidD, NULL);
    pthread_join(tidE, NULL);
    pthread_join(tidF, NULL);
    
	printf("counter: %d\n", counter);
	
    lthread_mutex_destroy(&mutex_lock_st);
	
    return 0;
}


void *doit(void *arg)
{    
    int i, val;
	uint64_t tmp = 0;
	uint64_t tid = pthread_self();
	
	printf("tid:%x - %ld\n", (unsigned int)tid, (unsigned int)tid);
	
    for (i=0; i<NLOOP; i++) {
        //pthread_mutex_lock(&counter_mutex);
		
		lthread_mutex_lock(&mutex_lock_st);

		counter += 1;

		lthread_mutex_unlock(&mutex_lock_st);
        //pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}
