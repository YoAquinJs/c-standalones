/*
Introductions to Algorithms
implementation for excersice 2,1-5
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

int NumInversionsMerge(void* buffer, void* buffer_cp, size_t memb_size,
                       Compare compare, size_t left, size_t mid, size_t right) {
    // copy left and right to buffer_cp
    memcpy(INDEX(buffer_cp, left, memb_size), INDEX(buffer, left, memb_size),
           (1 + mid - left) * memb_size);
    memcpy(INDEX(buffer_cp, mid + 1, memb_size),
           INDEX(buffer, mid + 1, memb_size), (right - mid) * memb_size);

    void* buffer_ptr = INDEX(buffer, left, memb_size);
    size_t inversions = 0, left_count = mid - left + 1;
    size_t _left = left, _right = mid + 1;

    // merge left and right to sort and count inversions
    while (_left <= mid && _right <= right) {
        if (compare(INDEX(buffer_cp, _right, memb_size),
                    INDEX(buffer_cp, _left, memb_size)) < 0) {
            memcpy(buffer_ptr, INDEX(buffer_cp, _left, memb_size), memb_size);
            inversions += left_count;
        } else {
            memcpy(buffer_ptr, INDEX(buffer_cp, _right, memb_size), memb_size);
            left_count--;
        }
        buffer_ptr = INDEX(buffer_ptr, 1, memb_size);
    }

    if (_left < mid) {
        memcpy(buffer_ptr, INDEX(buffer_cp, _left, memb_size),
               (1 + mid - _left) * memb_size);
    } else {
        memcpy(buffer_ptr, INDEX(buffer_cp, _right, memb_size),
               (right - mid) * memb_size);
    }

    return inversions;
}

int NumInversionsRecursion(void* buffer, void* buffer_cp, size_t memb_size,
                           Compare compare, size_t left, size_t right) {
    if (left >= right) return 0;

    int result = 0;

    int mid = left + (right - left) / 2;
    result += NumInversionsRecursion(buffer, buffer_cp, memb_size, compare,
                                     left, mid);
    result += NumInversionsRecursion(buffer, buffer_cp, memb_size, compare,
                                     mid + 1, right);
    result += NumInversionsMerge(buffer, buffer_cp, memb_size, compare, left,
                                 mid, right);

    return result;
}

// as a sideeffect sorts buffer
int NumInversions(void* buffer, size_t memb_size, size_t size,
                  Compare compare) {
    void* buffer_cp = malloc(size * memb_size);
    if (!buffer_cp) return -1;

    size_t result = NumInversionsRecursion(buffer, buffer_cp, memb_size,
                                           compare, 0, size - 1);
    free(buffer_cp);

    return result;
}
