
#include <pthread.h>
#include <stdio.h>

#include <stdint.h>
#include <linux/types.h>

#define NLOOP 50000000


static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint64_t counter = 0;

void *doit(void *);


int main()
{
    pthread_t tidA, tidB, tidC, tidD, tidE, tidF;
    
    printf("main pid:%x - %ld\n", (unsigned int)pthread_self(), (unsigned int)pthread_self());
    
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
    
    return 0;
}


void *doit(void *arg)
{    
    uint64_t i, val;
    uint64_t tid = pthread_self();
    
    printf("tid:%x - %ld\n", (unsigned int)tid, (unsigned int)tid);
    
    for (i=0; i<NLOOP; i++) {
        pthread_mutex_lock(&counter_mutex);
        
        counter += 1;
        
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}
