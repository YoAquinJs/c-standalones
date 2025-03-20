#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// bit block
typedef unsigned char bit_block;

#define BITS_IN_BYTE 8
#define BITS_IN_BLOCK (sizeof(bit_block) * BITS_IN_BYTE)

// calcs the count of bit blocks needed for size bits
#define BIT_BLOCK_COUNT(size)                                                  \
    (((size) / BITS_IN_BLOCK) + ((size) % BITS_IN_BLOCK == 0 ? 0 : 1))

// a < b  <-> int < 0
// a > b  <-> int > 0
// a == b <-> int == 0
typedef int8_t (*Compare)(const void *, const void *);

// index i-th index of raw mem buffer, with elem size given by memb_size
#define INDEX(buffer, i, memb_size)                                            \
    ((void *)((unsigned char *)(buffer) + ((i) * (memb_size))))

// swap elements in a buffer by indexes a and b
void swap(void *buffer, size_t memb_size, size_t a, size_t b);

#endif /* ifndef UTILS_H */
