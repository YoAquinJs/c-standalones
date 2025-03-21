#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

size_t CapacityDouble(size_t capacity) {
    return capacity == 0 ? 1 : capacity * 2;
}

size_t CapacityBinaryCompleteTree(size_t capacity) { return 1 + capacity * 2; }

Vector NewVectorFromBuf(void *buffer, size_t memb_size, size_t size,
                        CapacityIncrease capacity_inc) {
    if (!buffer || size == 0) {
        return NewVector(memb_size, size, capacity_inc);
    }

    void *memb_buffer = malloc(memb_size);
    return (Vector){buffer, memb_buffer, memb_size, size, size, capacity_inc};
}

Vector NewVector(size_t memb_size, size_t size, CapacityIncrease capacity_inc) {
    if (size == 0) {
        size = 1;
    }

    void *buffer = malloc(size * memb_size);
    void *memb_buffer = malloc(memb_size);
    return (Vector){buffer, memb_buffer, memb_size, 0, size, capacity_inc};
}

void FreeVector(Vector *vec) {
    free(vec->buffer);
    free(vec->memb_buffer);
    vec->buffer = NULL;
    vec->capacity = 0;
    vec->size = 0;
}

int VectorIndex(Vector *vec, size_t index, void **elem) {
    *elem = vec->buffer + (index * vec->memb_size);
    return index < vec->size ? 0 : 1;
}

bool VectorCheckOverflow(Vector *vec, size_t new_elems) {
    return vec->size + new_elems > vec->capacity;
}

int VectorCapacityOverflow(Vector *vec, size_t new_elems) {
    size_t capacity = vec->capacity, size = vec->size + new_elems;

    while (size > capacity) {
        capacity = vec->capacity_inc(capacity);
    }

    vec->capacity = capacity;
    vec->buffer = realloc(vec->buffer, vec->memb_size * capacity);

    return vec->buffer ? 0 : 1;
}

int VectorRemove(Vector *vec, size_t index) {
    if (index == vec->size - 1) {
        vec->size--;
        return 0;
    }

    void *index_ptr, *next_index_ptr;
    if (VectorIndex(vec, index, &index_ptr) == 1 ||
        VectorIndex(vec, index + 1, &next_index_ptr) == 1) {
        return 1;
    }

    memcpy(index_ptr, next_index_ptr, vec->memb_size * (vec->size - index - 1));

    vec->size--;
    return 0;
}

int VectorInsert(Vector *vec, void *elem, size_t index) {
    if (VectorCheckOverflow(vec, 1)) {
        if (VectorCapacityOverflow(vec, 1) == 1) {
            return 1;
        }
    }

    void *index_ptr;
    if (VectorIndex(vec, index, &index_ptr) == 1) {
        memcpy(index_ptr, elem, vec->memb_size);
        vec->size++;
        return 0;
    }

    void *next_index_ptr;
    VectorIndex(vec, index + 1, &next_index_ptr);
    memcpy(next_index_ptr, index_ptr, vec->memb_size * (vec->size - index));
    memcpy(index_ptr, elem, vec->memb_size);

    vec->size++;
    return 0;
}

void VectorClear(Vector *vec) {
    vec->size = 0;
    /*memset(vec->buffer, 0, vec->memb_size * vec->capacity);*/
}

int VectorSwap(Vector *vec, size_t index_a, size_t index_b) {
    void *a, *b;
    if (VectorIndex(vec, index_a, &a) == 1 || VectorIndex(vec, index_b, &b)) {
        return 1;
    }

    memcpy(vec->memb_buffer, a, vec->memb_size);
    memcpy(a, b, vec->memb_size);
    memcpy(b, vec->memb_buffer, vec->memb_size);

    return 0;
}

int VectorAppend(Vector *vec, void *elem) {
    return VectorInsert(vec, elem, vec->size);
}

int VectorPop(Vector *vec) { return VectorRemove(vec, vec->size - 1); }
