#include <pthread.h>
#include <unistd.h>

#include <stdio.h>

int turn;
int flag[2];

int shared;

void *f0(void *arg)
{
    while (1)
    {
        // Signal that this thread wants to enter the critical section.
        flag[0] = 1;
        // Signal to the other thread that it is their turn.
        turn = 1;
        while (flag[1] && turn == 1)
        {
        };
        shared = shared + 1;
        printf("Turn of process: %d\n", turn);
        printf("%d\n", shared);
        puts("hello");
        printf("End of process: %d\n\n", turn);
        flag[0] = 0;
        sleep(1);
    }
}

void *f1(void *arg)
{
    while (1)
    {
        flag[1] = 1;
        turn = 0;
        while (flag[0] && turn == 0)
        {
        };
        shared = shared + 1;
        printf("Turn of process: %d\n", turn);
        printf("%d\n", shared);
        puts("world");
        printf("End of process: %d\n\n", turn);
        flag[1] = 0;
        sleep(1);
    }
}

int main(void)
{
    // A POSIX thread has two main components: an object of type `pthread_t`
    // which represents the thread and a function pointer of type
    // `void* (*)(void*)` which will be the entry point of the thread.
    pthread_t t0, t1;

    // Creates new threads. The second argument is a pointer to a
    // `pthread_attr_t`, if `NULL` the thread is created with default attributes.
    // The last argument is the argument that is given to the thread's entry
    // point function, unused in this example.
    pthread_create(&t0, NULL, f0, NULL);
    pthread_create(&t1, NULL, f1, NULL);

    // Yes, I could have just created one thread.
    while (1)
        ;
}