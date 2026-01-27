#include <stdio.h>
#include "bmalloc.h"
#include <string.h>

int main(int argc, char** argv) {


    printf("Test 1: Basic allocation\n");

    char* ptr1 = bmalloc(100);
    if (ptr1 == NULL) {
        printf("FAIL: allocation returned NULL\n");
        return 1;
    }

    printf("PASS: allocated 100 bytes at %p\n", ptr1);

    strcpy(ptr1, "Hello, world!");
    printf("PASS: wrote and read: %s\n", ptr1);

    bfree(ptr1);
    printf("PASS: freed ptr1\n\n");

    return 0;
}
