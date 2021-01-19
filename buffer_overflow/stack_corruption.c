#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Run it without a stack protector to see the overflow
int main(void)
{
    // Buffers have size of 16 bytes
    char buf2[16] = "overwriteme";
    char buf1[16];

    // Copy to buf2 something with larger size than 16 bytes
    strcpy(buf2, "12345679808123412342314");

    printf("buf1 val: %s\n", buf1);
    printf("buf2 val: %s\n", buf2);
    printf("buf1 addr: %p\n", (void *)buf1);
    printf("buf2 addr: %p\n", (void *)buf2);
}
