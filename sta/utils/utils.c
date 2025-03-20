#include "utils.h"

#include <stdio.h>
#include <string.h>

#define SWAP_BUF_SIZE 64

void swap(void *buffer, size_t memb_size, size_t a, size_t b) {
    if (a == b)
        return;

    unsigned char buf[SWAP_BUF_SIZE];

    unsigned char *p1 = INDEX(buffer, a, memb_size);
    unsigned char *p2 = INDEX(buffer, b, memb_size);

    size_t left = memb_size, copy_size;
    while (left) {
        copy_size = left > memb_size ? memb_size : left;

        memcpy(buf, p1, copy_size);
        memcpy(p1, p2, copy_size);
        memcpy(p2, buf, copy_size);

        p1 += copy_size;
        p2 += copy_size;
    }
}
