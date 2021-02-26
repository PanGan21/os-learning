#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 3 // Size of shared buffer

int buffer[BUF_SIZE]; // Shared buffer
int add = 0;          // Index to add next element
int rem = 0;          // Index to remove next element
int num = 0;          // Number of elements in the buffer

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;    // Mutex lock for buffer
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; // Consumer waits on condition variable
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; // Producer waits on condition variable

void *producer(void *param);
void *consumer(void *param);

void *producer(void *param)
{
    int i;
    for (i = 1; i <= 20; i++)
    {
        // Insert into buffer
        pthread_mutex_lock(&m);

        if (num > BUF_SIZE) // Overflow
        {
            exit(1);
        }
        while (num == BUF_SIZE) // Block if buffer is full
        {
            pthread_cond_wait(&c_prod, &m);
        }
        buffer[add] = i; // Buffer not full, so add element
        add = (add + 1) % BUF_SIZE;
        num++;
        printf("Producer: inserted %d\n", i);
        printf("Producer: buffer size: %d\n\n", num);

        pthread_mutex_unlock(&m);

        pthread_cond_signal(&c_cons);
        fflush(stdout);
    }

    printf("****Producer quiting****\n\n");
    fflush(stdout);
    return 0;
}

// The consumer never terminates because it operates in a continuous while(1) loop
void *consumer(void *param)
{
    int i;

    while (1)
    {
        pthread_mutex_lock(&m);

        if (num < 0) // Underflow
        {
            exit(1);
        }
        while (num == 0) // Block if buffer is empty
        {
            pthread_cond_wait(&c_cons, &m);
        }
        i = buffer[rem]; // Buffer not empty so remove element
        rem = (rem + 1) % BUF_SIZE;
        num--;

        printf("Consumer: removed value %d\n", i);
        printf("Consumer: buffer size: %d\n\n", num);
        pthread_mutex_unlock(&m);

        pthread_cond_signal(&c_prod);
        fflush(stdout);
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    pthread_t tid1, tid2; // Thread identifiers
    int i;

    if (pthread_create(&tid1, NULL, producer, NULL) != 0)
    {
        fprintf(stderr, "Unable to create producer thread\n");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, consumer, NULL) != 0)
    {
        fprintf(stderr, "Unable to create consumer thread\n");
        exit(1);
    }

    pthread_join(tid1, NULL); // Wait for producer to exit
    pthread_join(tid2, NULL); // Wait for consumer to exit
    printf("Parent quiting\n");

    return 0;
}
