#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNT_TO 10000
#define MAX_CORES 12

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long long i = 0;

void *start_consuming(void *arg)
{
    for (;;)
    {
        pthread_mutex_lock(&mutex);

        // Critical section start
        if (i >= COUNT_TO)
        {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        ++i;
        // Critical section end

        pthread_mutex_unlock(&mutex);

        printf("i = %lld\n", i);
    }
}

int main(void)
{
    int i = 0;

    pthread_t *thread_group = malloc(sizeof(pthread_t) * MAX_CORES);

    for (i = 0; i < MAX_CORES; i++)
    {
        pthread_create(&thread_group[i], NULL, start_consuming, NULL);
    }

    for (i = 0; i < MAX_CORES; i++)
    {
        pthread_join(thread_group[i], NULL);
    }

    return EXIT_SUCCESS;
}
