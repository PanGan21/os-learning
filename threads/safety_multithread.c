#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
// #include <stdarg.h>
// #include <errno.h>
// #include <sys/types.h>
// #include <signal.h>
// #include <sys/time.h>
// #include <sys/ioctl.h>

// standsrd HTTP port
#define SERVER_PORT 80

#define MAX_LINE 4096
#define SA struct sockaddr

#define BIG 1000000000UL

uint32_t counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *count_to_big(void *arg)
{
    for (uint32_t i = 0; i < BIG; i++)
    {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

char *getHomePage(char *ipaddr, int *bytesread)
{
    int sockfd, n;
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendline[MAX_LINE];
    char *buffer = malloc(MAX_LINE);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return NULL;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = SERVER_PORT;

    if (inet_pton(AF_INET, ipaddr, &servaddr.sin_addr) <= 0)
    {
        return NULL;
    }

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) <= 0)
    {
        return NULL;
    }

    // Connected. Prepare the message
    sprintf(sendline, "GET / HTTP/1.1\r'n'r'n");
    sendbytes = strlen(sendline);

    // Send the request
    if (write(sockfd, sendline, sendbytes) != sendbytes)
    {
        return NULL;
    }

    memset(buffer, 0, MAX_LINE);

    // Read the first MAXLINE server's response
    n = read(sockfd, buffer, MAX_LINE - 1);
    if (n < 0)
    {
        return NULL;
    }

    *bytesread = n;
    return buffer;
}

int main()
{
    pthread_t t;
    char *buf;
    int buflen;

    pthread_create(&t, NULL, count_to_big, NULL);

    buf = getHomePage("172.217.0.78", &buflen);
    pthread_join(t, NULL);
    printf("Done. Counter = %u. Recved %d bytes\n", counter, buflen);
}
