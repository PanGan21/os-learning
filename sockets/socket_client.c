#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDR "localhost"
#define SERVER_PORT 8888

int main(int argc, char const *argv[])
{
    int socket_fd = 0;
    struct sockaddr_in server_sock_addr;

    // Convert localhost to 0.0.0.0
    struct hostent *he = gethostbyname(SERVER_ADDR);
    // struct hostent *he = gethostbyname(SERVER_ADDR);
    unsigned long server_addr_nbo = *(unsigned long *)(he->h_addr_list[0]);

    // Create socket (IPv4, stream-based, protocol likely set to TCP)
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "client failed to create socket\n");
        exit(1);
    }

    // Configure server socket address structure (init to zero, IPv4, network byte order for port and address)
    bzero(&server_sock_addr, sizeof(server_sock_addr));
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = htons(SERVER_PORT);
    server_sock_addr.sin_addr.s_addr = server_addr_nbo;

    // Connect socket to the server
    if (connect(socket_fd, (struct sockaddr *)&server_sock_addr, sizeof(server_sock_addr)) < 0)
    {
        fprintf(stderr, "client failed to connect to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
        close(socket_fd);
        exit(1);
    }
    else
    {
        fprintf(stdout, "client connected to to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
    }

    // Close the socket and return
    close(socket_fd);
    return 0;
}
