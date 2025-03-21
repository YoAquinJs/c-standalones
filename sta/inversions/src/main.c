/*
Introductions to Algorithms
implementation for excersice 2,1-5

Computes the number of inversions in a list of ints, an inversion is counted
for every i and j in the list, such that i > j, and ith < jth.

Reads the list of ints from stdinput and ignores any non valid int.
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "inversions.h"
#include "vector.h"

#define NUMS_INIT_SIZE 8

FILE *in_stream;

int main() {
    in_stream = stdin;

    int num;
    Vector nums_list = NewVector(sizeof(int), NUMS_INIT_SIZE, CapacityDouble);

    size_t line_size;
    char *lineptr = NULL, *str;

    while (getline(&lineptr, &line_size, in_stream) != EOF) {
        int read_num_size;
        str = lineptr;

        while (sscanf(str, "%d%n", &num, &read_num_size) == 1) {
            VectorAppend(&nums_list, &num);
            str += read_num_size;
        }
    }

    int inversions = NumInversions(nums_list.buffer, nums_list.memb_size,
                                   nums_list.size, CompareInt);
    printf("inversions: %d\n", inversions);

    if (lineptr)
        free(lineptr);
    FreeVector(&nums_list);

    return EXIT_SUCCESS;
}
