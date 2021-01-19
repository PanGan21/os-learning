#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* myturn(void* arg) {
    for (int i = 0; i < 8; i++)
    {
        sleep(1);
        printf("My turn! %d\n", i);
        
    }
    return NULL;
}

void yourturn() {
    for (int i = 0; i < 3; i++)
    {
        sleep(2);
        printf("Your turn! %d\n", i);
        
    }
    
}

int main() {
    pthread_t newthread;
    
    pthread_create(&newthread, NULL, myturn, NULL);

    yourturn();

    // Wait until the tread is done before exit
    pthread_join(newthread, NULL);
}