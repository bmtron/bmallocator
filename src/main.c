#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "bmalloc.h"

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

        printf("Test 2: Catching double frees\n");

        char* ptr2 = bmalloc(100);
        if (ptr2 == NULL) {
                printf("FAIL: allocation returned NULL\n");
                return 1;
        }

        printf("PASS: allocated 100 bytes at %p\n", ptr2);
        strcpy(ptr2, "Hello, again!");
        printf("PASS: wrote and read: %s\n", ptr2);

        bfree(ptr2);
        printf("PASS: freed ptr2 once\n");

        bfree(ptr2);
        if (errno == EINVAL) {
                perror("bfree");
                printf("PASS: successfully set errno on doublefree\n");
        }

        return 0;
}
