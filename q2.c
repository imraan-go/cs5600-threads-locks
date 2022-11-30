
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "common_threads.h"
typedef struct __counter_t
{
    int value;
    pthread_mutex_t lock;
} counter_t;
// typedef struct __args
// {
//     int limit;
//     char *message;
// };

void init(counter_t *c)
{
    c->value = 0;
    Pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c)
{
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c)
{
    Pthread_mutex_lock(&c->lock);
    c->value--;
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c)
{
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

counter_t *concurrent_counter;
void *do_task(void *ptr)
{
    char *message;
    message = (char *)ptr;

    for (int i = 0; i < 5000000; i++)
    {
        increment(concurrent_counter);
    }
    printf("%s -  value: %d \n", message, get(concurrent_counter));
}
int main()
{
    concurrent_counter = malloc(sizeof(counter_t));
    init(concurrent_counter);

    pthread_t thread1,
        thread2, thread3, thread4;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
    char *message3 = "Thread 3";
    char *message4 = "Thread 4";
    int iret1, iret2, iret3, iret4;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    /* Create independent threads each of which will execute function */

    iret1 = pthread_create(&thread1, NULL, do_task, (void *)message1);
    iret2 = pthread_create(&thread2, NULL, do_task, (void *)message2);
    iret3 = pthread_create(&thread3, NULL, do_task, (void *)message3);
    iret4 = pthread_create(&thread4, NULL, do_task, (void *)message4);

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
