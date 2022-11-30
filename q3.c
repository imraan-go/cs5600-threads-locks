
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "common_threads.h"
#define NUMCPUS 4

typedef struct __counter_t
{
    int
        global;
    // global count
    pthread_mutex_t glock;
    // global lock
    int
        local[NUMCPUS];             // per-CPU count
    pthread_mutex_t llock[NUMCPUS]; // ... and locks
    int
        threshold;
    // update frequency
} counter_t;

void init(counter_t *c, int threshold)
{
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    int i;
    for (i = 0; i < NUMCPUS; i++)
    {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

// update: usually, just grab local lock and update
// local amount; once local count has risen ’threshold’,
// grab global lock and transfer local values to it
void update(counter_t *c, int threadID, int amt)
{
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold)
    {
        // transfer to global (assumes amt>0)
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

// get: just return global amount (approximate)
int get(counter_t *c)
{
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val; // only approximate!
}

counter_t *concurrent_counter;
void *do_task(void *ptr)
{
    int *message;
    message = (int *)ptr;
    pid_t tid = syscall(SYS_gettid);
    for (int i = 0; i < 5000000; i++)
    {
        update(concurrent_counter, tid, 1);
    }
    printf("Thread %d -  value: %d \n", tid % NUMCPUS, get(concurrent_counter));
}
int main()
{
    concurrent_counter = malloc(sizeof(counter_t));
    init(concurrent_counter, 50000);

    pthread_t thread1,
        thread2, thread3, thread4;
    int message1 = 1;
    int message2 = 2;
    int message3 = 3;
    int message4 = 4;
    int iret1, iret2, iret3, iret4;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    /* Create independent threads each of which will execute function */

    iret1 = pthread_create(&thread1, NULL, do_task, (void *)&message1);
    iret2 = pthread_create(&thread2, NULL, do_task, (void *)&message2);
    iret3 = pthread_create(&thread3, NULL, do_task, (void *)&message3);
    iret4 = pthread_create(&thread4, NULL, do_task, (void *)&message4);

    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    gettimeofday(&end_time, NULL);
    double elapsed = (end_time.tv_sec - start_time.tv_sec) +
                     (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Took seconds %f \n", elapsed);

    printf("Final value: %d \n", get(concurrent_counter));
    return 0;
}
