#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void *myturn(void *arg)
{
    int *iptr = (int *)malloc(sizeof(int));
    *iptr = 5;
    for (int i = 0; i < 8; i++)
    {
        sleep(1);
        printf("My turn! %d %d\n", i, *iptr);
        (*iptr)++;
    }
    return iptr;
}

void yourturn()
{
    for (int i = 0; i < 3; i++)
    {
        sleep(2);
        printf("Your turn! %d\n", i);
    }
}

int main()
{
    pthread_t newthread;
    int *result;

    pthread_create(&newthread, NULL, myturn, NULL);

    yourturn();

    // Wait until the tread is done before exit
    pthread_join(newthread, (void *)&result);
    printf("Thread is done *result=%d\n", *result);
}