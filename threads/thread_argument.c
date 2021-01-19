#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 8

void *threadFunc(void *pArg)
{
    int *p = (int *)pArg;
    int myNum = *p;
    printf("Thread number %d\n", myNum);
    return 0;
}

int main(void)
{
    int i;
    int tNum[NUM_THREADS];
    pthread_t tid[NUM_THREADS];

    // Create - Fork threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        tNum[i] = i;
        pthread_create(&tid[i], NULL, threadFunc, &tNum[i]);
    }

    // Wait - join threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}