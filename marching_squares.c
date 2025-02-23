#include "image_utils.h"
#include "marching_squares.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4 // Pattern size

// Free the memory for the binary grid
void free_grid(int **grid, int n)
{
	for (int i = 0; i < n; i++) {
		if (grid[i])
			free(grid[i]);
	}
	free(grid);
}

void print_grid(int **grid, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", grid[i][j]);
		}
		printf("\n");
	}
}

int calculate_luminosity(pixel_struct pixel)
{
	return (pixel.red + pixel.green + pixel.blue) / 3;
}

// Compute luminosity matrix
int **calculate_lumin_matrix(image_struct *image)
{
	int **lumin_matrix = malloc(image->height * sizeof(int *));
	for (int i = 0; i < image->height; i++) {
		lumin_matrix[i] = malloc(image->width * sizeof(int));
	}

	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->width; j++) {
			lumin_matrix[i][j] = calculate_luminosity(image->pixels[i][j]);
		}
	}
	return lumin_matrix;
}

// Compute binary grid
int **calculate_grid(image_struct *image, int sampling_value, int threshold)
{
	// Compute size of the grid
	int n = image->width / sampling_value + 1;

	// Allocate memory for the grid G
	int **grid = malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++) {
		grid[i] = calloc(n, sizeof(int));
	}

	int **lumin_matrix = calculate_lumin_matrix(image);

	// Compute grid values
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int lumin_x = i * sampling_value;
			int lumin_y = j * sampling_value;

			int sum = 0;
			int count = 0;

			// Current point
			if (lumin_x < image->height && lumin_y < image->width) {
				sum += lumin_matrix[lumin_x][lumin_y];
				count++;
			}

			// Neighbors on the diagonals
			// Nord-West
			if (lumin_x - 1 >= 0 && lumin_y - 1 >= 0) {
				sum += lumin_matrix[lumin_x - 1][lumin_y - 1];
				count++;
			}

			// Nord-East
			if (lumin_x - 1 >= 0 && lumin_y + 1 < image->width) {
				sum += lumin_matrix[lumin_x - 1][lumin_y + 1];
				count++;
			}

			// South-West
			if (lumin_x + 1 < image->height && lumin_y - 1 >= 0) {
				sum += lumin_matrix[lumin_x + 1][lumin_y - 1];
				count++;
			}

			// South-East
			if (lumin_x + 1 < image->height && lumin_y + 1 < image->width) {
				sum += lumin_matrix[lumin_x + 1][lumin_y + 1];
				count++;
			}

			// Compute average
			grid[i][j] = sum / count;
		}
	}

	// We apply the threshold to make the grid binary
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (grid[i][j] > threshold) {
				grid[i][j] = 0;
			} else {
				grid[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < image->height; i++) {
		free(lumin_matrix[i]);
	}
	free(lumin_matrix);

	return grid;
}

// Transform binary number to decimal configuration number
int get_configuration_number(int **grid, int i, int j)
{
	int config = grid[i][j] * 8 +
				grid[i][j + 1] * 4 +
				grid[i + 1][j + 1] * 2 +
				grid[i + 1][j];
	return config;
}

// Function to copy a 4x4 pattern into a larger image
void place_pattern_in_image(image_struct *image, pattern_struct *pattern,
							int start_row, int start_col)
{
	if (!image) {
		printf("Eroare: Alocare memorie eșuată.\n");
		return;
	}
	if (!pattern) {
		printf("Eroare: Alocare memorie eșuată.\n");
		return;
	}
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// Place the pattern pixel into the corresponding image pixel
			image->pixels[start_row + i][start_col + j] = pattern->pixels[i][j];
		}
	}
}

image_struct *marching_squares
(int **binary_grid, pattern_struct **lookup_table, int height, int width)
{
	image_struct *image = malloc(sizeof(image_struct));
	if (!image) {
		printf("Eroare: Alocare memorie eșuată.\n");
		return 0;
	}
	image->width = width;
	image->height = height;
	image->max_color = 255;

	image->pixels = calloc(height, sizeof(pixel_struct *));
	if (!image->pixels) {
		printf("Eroare: Alocare memorie pentru pixeli eșuată.\n");
		free(image);
		return 0;
	}
	for (int i = 0; i < image->height; i++) {
		image->pixels[i] = calloc(image->width, sizeof(pixel_struct));
	}

	int n = height / GRID_STEP;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int config = get_configuration_number(binary_grid, i, j);
			place_pattern_in_image
			(image, lookup_table[config], i * SIZE, j * SIZE);
		}
	}

	return image;
}
