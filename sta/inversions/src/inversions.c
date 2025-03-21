#include "inversions.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static int NumInvMerge(void *buffer, void *buffer_cp, size_t memb_size,
                       Compare compare, size_t left, size_t mid, size_t right) {
    size_t inversions = 0, left_count = mid - left + 1;
    void *buffer_ptr = INDEX(buffer, left, memb_size);
    size_t left_ptr = left, right_ptr = mid + 1;

    // copy left and right to buffer_cp
    memcpy(INDEX(buffer_cp, left, memb_size), INDEX(buffer, left, memb_size),
           left_count * memb_size);
    memcpy(INDEX(buffer_cp, mid + 1, memb_size),
           INDEX(buffer, mid + 1, memb_size), (right - mid) * memb_size);

    // merge left and right to sort and count inversions
    while (left_ptr <= mid && right_ptr <= right) {
        void *left_elem = INDEX(buffer_cp, left_ptr, memb_size);
        void *right_elem = INDEX(buffer_cp, right_ptr, memb_size);

        if (compare(right_elem, left_elem) < 0) {
            memcpy(buffer_ptr, right_elem, memb_size);
            inversions += left_count;
            right_ptr++;
        } else {
            memcpy(buffer_ptr, left_elem, memb_size);
            left_count--;
            left_ptr++;
        }
        buffer_ptr = INDEX(buffer_ptr, 1, memb_size);
    }

    if (left_count) {
        memcpy(buffer_ptr, INDEX(buffer_cp, left_ptr, memb_size),
               (1 + mid - left_ptr) * memb_size);
    } else {
        memcpy(buffer_ptr, INDEX(buffer_cp, right_ptr, memb_size),
               (right - mid) * memb_size);
    }

    return inversions;
}

static int NumInvRecursion(void *buffer, void *buffer_cp, size_t memb_size,
                           Compare compare, size_t left, size_t right) {
    if (left >= right)
        return 0;

    int result = 0;

    int mid = left + (right - left) / 2;
    result += NumInvRecursion(buffer, buffer_cp, memb_size, compare, left, mid);
    result +=
        NumInvRecursion(buffer, buffer_cp, memb_size, compare, mid + 1, right);
    result +=
        NumInvMerge(buffer, buffer_cp, memb_size, compare, left, mid, right);

    return result;
}

// sorts buffer as side effect
int NumInversions(void *buffer, size_t memb_size, size_t size,
                  Compare compare) {
    if (size == 0)
        return -1;

    void *buffer_cp = malloc(size * memb_size);
    if (!buffer_cp)
        return -1;

    size_t result =
        NumInvRecursion(buffer, buffer_cp, memb_size, compare, 0, size - 1);
    free(buffer_cp);

    return result;
}
