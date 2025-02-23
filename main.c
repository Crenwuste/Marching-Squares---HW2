#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_utils.h"
#include "marching_squares.h"

int main(void)
{
	char command[15];
	image_struct *image = NULL;
	int **binary_grid = NULL;
	int prev_n = 0;
	pattern_struct **lookup_table = NULL;

	while (1) {
		scanf("%s", command);
		if (strcmp(command, "READ") == 0) {
			if (image) {
				prev_n = image->width / GRID_STEP + 1;
				if (binary_grid) {
					free_grid(binary_grid, prev_n);
					binary_grid = NULL;
				}
				free_image(image);
			}
			image = read_ppm();
			if (image) {
				printf("Imagine citita cu succes ");
				printf("[%d-%d]\n", image->height, image->width);
			} else {
				return 0;
			}
		} else if (strcmp(command, "WRITE") == 0) {
			write_ppm(image);
		} else if (strcmp(command, "GRID") == 0) {
			binary_grid = calculate_grid(image, GRID_STEP, THRESHOLD);
			printf("Grid calculat cu succes ");
			printf("[%d-",	image->height / GRID_STEP);
			printf("%d]\n", image->width / GRID_STEP);
			print_grid(binary_grid, image->width / GRID_STEP);
		} else if (strcmp(command, "RESIZE") == 0) {
			image_struct *resized_image = resize_image(image, RESIZE_FACTOR);
			free_image(image);
			image = resized_image;
			printf("Imagine redimensionata cu succes ");
			printf("[%d-%d]\n", image->width, image->height);
		} else if (strcmp(command, "INIT_CONTUR") == 0) {
			lookup_table = initialize_contour_patterns();
			print_contur(lookup_table);
		} else if (strcmp(command, "MARCH") == 0) {
			if (!lookup_table) {
				lookup_table = initialize_contour_patterns();
			}
			if (!binary_grid) {
				binary_grid = calculate_grid(image, GRID_STEP, THRESHOLD);
			}
			image_struct *image_after_march = marching_squares(binary_grid,
																lookup_table,
																image->height,
																image->width);
			free_image(image);
			image = image_after_march;
			printf("Marching Squares aplicat cu succes ");
			printf("[%d-%d]\n", image->width, image->height);
		} else if (strcmp(command, "EXIT") == 0) {
			printf("Gigel a terminat\n");
			break;
		}
	}

	if (binary_grid) {
		free_grid(binary_grid, image->width / GRID_STEP + 1);
	}
	if (image) {
		free_image(image);
	}
	if (lookup_table) {
		free_patterns(lookup_table);
	}

	return 0;
}
