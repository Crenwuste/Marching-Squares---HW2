#ifndef MARCH_SQUARES_H
#define MARCH_SQUARES_H

#include "image_utils.h"

void print_grid(int **grid, int N);
void free_grid(int **grid, int N);
int **calculate_grid(image_struct *image, int sampling_value, int threshold);
image_struct *marching_squares
(int **binary_grid, pattern_struct **lookup_table, int height, int width);

#endif
