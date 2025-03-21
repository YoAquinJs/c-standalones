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

#define COMPARE_BTYPE_IMPL(type, func_name)                                    \
    COMPARE_BTYPE_DECL(type, func_name) {                                      \
        const type arg1 = *((type *)a);                                        \
        const type arg2 = *((type *)b);                                        \
        if (arg1 == arg2)                                                      \
            return 0;                                                          \
        return arg1 - arg2;                                                    \
    }

COMPARE_BTYPE_IMPL(char, CompareChar);
COMPARE_BTYPE_IMPL(unsigned char, CompareUChar);
COMPARE_BTYPE_IMPL(int, CompareInt);
COMPARE_BTYPE_IMPL(unsigned int, CompareUInt);
COMPARE_BTYPE_IMPL(long, CompareLong);
COMPARE_BTYPE_IMPL(unsigned long, CompareULong);
COMPARE_BTYPE_IMPL(float, CompareFloat);
COMPARE_BTYPE_IMPL(double, CompareDouble);
