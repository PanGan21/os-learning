#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Stack protector is not working for the heap
int main(void)
{
    // Buffers have size of 12 bytes
    char *buf1 = malloc(12 * sizeof(char));
    char *buf2 = malloc(12 * sizeof(char));

    // Copy to buf2 something with larger size than 12 bytes
    strcpy(buf2, "mywordshere");
    strcpy(buf1, "123412341234123412341234x");

    printf("buf1 val: %s\n", buf1);
    printf("buf2 val: %s\n", buf2);
    printf("buf1 addr: %p\n", (void *)buf1);
    printf("buf2 addr: %p\n", (void *)buf2);
}
