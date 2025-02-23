#include "image_utils.h"
#include "marching_squares.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function for reading the PPM file
image_struct *read_ppm(void)
{
	image_struct *image = malloc(sizeof(image_struct));
	if (!image) {
		fprintf(stderr, "Eroare: Alocare memorie eșuată pentru imagine\n");
		return NULL;
	}
	// Read PPM format
	char format[3];
	if (fscanf(stdin, "%2s", format) != 1 || strcmp(format, "P3") != 0) {
		printf("Eroare: trebuie sa fie P3\n");
		free(image);
		return NULL;
	}
	// Reading the image dimensions and the maximum color value
	scanf("%d %d %d", &image->width, &image->height, &image->max_color);

	// Allocate memory for pixels
	image->pixels = malloc(image->height * sizeof(pixel_struct *));
	if (!image->pixels) {
		fprintf(stderr, "Eroare: Alocare memorie eșuată pentru pixeli\n");
		free(image);
		return NULL;
	}
	for (int i = 0; i < image->height; i++) {
		image->pixels[i] = malloc(image->width * sizeof(pixel_struct));
		if (!image->pixels[i]) {
			printf("Eroare: Alocare memorie pentru rândul %d eșuată\n", i);

			// Free the previously allocated memory
			for (int k = 0; k < i; k++) {
				free(image->pixels[k]);
			}
			free(image->pixels);
			free(image);
			return NULL;
		}
		// Reading the RGB values for each pixel
		for (int j = 0; j < image->width; j++) {
			// Check if the reading failed
			if
			(fscanf(stdin, "%d %d %d", &image->pixels[i][j].red,
			&image->pixels[i][j].green, &image->pixels[i][j].blue) != 3) {
				printf("Eroare: eroare citire pixeli\n");
				// Free the allocated memory
				for (int k = 0; k <= i; k++) {
					free(image->pixels[k]);
				}
				free(image->pixels);
				free(image);
				return NULL;
			}
			// Check if the RGB values are within the valid range
			if (image->pixels[i][j].red < 0 ||
				image->pixels[i][j].red > image->max_color ||
				image->pixels[i][j].green < 0 ||
				image->pixels[i][j].green > image->max_color ||
				image->pixels[i][j].blue < 0 ||
				image->pixels[i][j].blue > image->max_color) {
				printf("Eroare: valoare pixel necorespunzatoare\n");
				// Free the allocated memory
				for (int k = 0; k <= i; k++) {
					free(image->pixels[k]);
				}
				free(image->pixels);
				free(image);
				return NULL;
			}
		}
	}
	return image;
}

// Function for writing the PPM image
void write_ppm(image_struct *image)
{
	// Write the PPM image header
	fprintf(stdout, "P3\n%d %d\n%d\n",
			image->width, image->height, image->max_color
			);

	// Write the image pixels
	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->width; j++) {
			fprintf(stdout, "%d %d %d\n",
					image->pixels[i][j].red,
					image->pixels[i][j].green,
					image->pixels[i][j].blue);
		}
	}
}

// Function for freeing the image memory
void free_image(image_struct *image)
{
	for (int i = 0; i < image->height; i++) {
		free(image->pixels[i]);
	}
	free(image->pixels);
	free(image);
}

// Function for resizing the image
image_struct *resize_image(image_struct *image, int factor)
{
	int new_width = image->width * factor;
	int new_height = image->height * factor;

	image_struct *resized_image = malloc(sizeof(image_struct));
	resized_image->width = new_width;
	resized_image->height = new_height;
	resized_image->max_color = image->max_color;

	resized_image->pixels = malloc(new_height * sizeof(pixel_struct *));
	for (int i = 0; i < new_height; i++) {
		resized_image->pixels[i] = malloc(new_width * sizeof(pixel_struct));
		for (int j = 0; j < new_width; j++) {
			resized_image->pixels[i][j] = image->pixels[i / factor][j / factor];
		}
	}
	return resized_image;
}

pixel_struct create_pixel(int value)
{
	return (pixel_struct){value, value, value};
}

// Function to fill the image with a single pixel value
void fill_pattern(pattern_struct *pattern, int value)
{
	pixel_struct pixel = create_pixel(value);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			pattern->pixels[i][j] = pixel;
		}
	}
}

// Function for generating a triangle
void generate_triangle(pattern_struct *pattern, int corner, int color)
{
	pixel_struct pixel_1 = create_pixel(color);
	pixel_struct pixel_2 = create_pixel(255);

	if (corner == 0) { // Top-left corner
		pattern->pixels[0][0] = pixel_1;
		pattern->pixels[0][1] = pixel_2;
		pattern->pixels[1][0] = pixel_2;
	} else if (corner == 1) { // Top-right corner
		pattern->pixels[0][3] = pixel_1;
		pattern->pixels[0][2] = pixel_2;
		pattern->pixels[1][3] = pixel_2;
	} else if (corner == 2) { // Bottom-right corner
		pattern->pixels[3][3] = pixel_1;
		pattern->pixels[3][2] = pixel_2;
		pattern->pixels[2][3] = pixel_2;
	} else if (corner == 3) { // Bottom-left corner
		pattern->pixels[3][0] = pixel_1;
		pattern->pixels[3][1] = pixel_2;
		pattern->pixels[2][0] = pixel_2;
	}
}

// Function for generating a rectangle
void generate_rectangle(pattern_struct *pattern, int corner1, int corner2)
{
	pixel_struct gray = create_pixel(180);
	pixel_struct black = create_pixel(255);
	if (corner1 == 0 && corner2 == 1) { // Up
		for (int j = 0; j < SIZE; j++) {
			pattern->pixels[0][j] = gray;
			pattern->pixels[1][j] = black;
		}
	} else if (corner1 == 1 && corner2 == 2) { // Right
		for (int i = 0; i < SIZE; i++) {
			pattern->pixels[i][2] = black;
			pattern->pixels[i][3] = gray;
		}
	} else if (corner1 == 2 && corner2 == 3) { // Down
		for (int j = 0; j < SIZE; j++) {
			pattern->pixels[2][j] = black;
			pattern->pixels[3][j] = gray;
		}
	} else if (corner1 == 0 && corner2 == 3) { // Left
		for (int i = 0; i < SIZE; i++) {
			pattern->pixels[i][0] = gray;
			pattern->pixels[i][1] = black;
		}
	}
}

// Function for generating a trapezoid
void generate_trapezoid(pattern_struct *pattern, int missing_corner)
{
	fill_pattern(pattern, 180); // Initialize the matrix with 0
	generate_triangle(pattern, missing_corner, 0);
}

// Function that generates the matrix based on the configuration
void generate_pattern(pattern_struct
**lookup_table, int config)
{
	// Specific pattern for the config
	pattern_struct *pattern = lookup_table[config];
	pattern->key = config;

	int corners[4] = {
		(config & 8) >> 3, // Top left corner
		(config & 4) >> 2, // Top right corner
		(config & 2) >> 1, // Down right corner
		(config & 1)       // Down left corner
	};

	int sum = corners[0] + corners[1] + corners[2] + corners[3];

	fill_pattern(pattern, 0);

	if (sum == 4) {
		// All corners 1 -> Matrix filled with 255
		fill_pattern(pattern, 255);
	} else if (sum == 1) {
		// Only one corner is 1 -> Triangle
		for (int i = 0; i < 4; i++) {
			if (corners[i] == 1) {
				generate_triangle(pattern, i, 180);
				break;
			}
		}
	} else if (sum == 2) {
		for (int i = 0; i < 4; i++) {
			for (int j = i + 1; j < 4; j++) {
				// Two adjacent corners are 1 -> Rectangle
				if (corners[i] == 1 && corners[j] == 1 && abs(i - j) != 2) {
					generate_rectangle(pattern, i, j);
					break;
				// Two opposite corners are 1-> Two triangles
				} else if (corners[i] == 1 &&
					corners[j] == 1 &&
					abs(i - j) == 2) {
					generate_triangle(pattern, i, 180);
					generate_triangle(pattern, j, 180);
					break;
				}
			}
		}
	} else if (sum == 3) {
		// Three corners are 1 -> Trapezoid
		for (int i = 0; i < 4; i++) {
			if (corners[i] == 0) {
				generate_trapezoid(pattern, i);
				break;
			}
		}
	}
}

void print_contur(pattern_struct **lookup_table)
{
	for (int k = 0; k < 16; k++) {
		fprintf(stdout, "P3\n");
		fprintf(stdout, "4 4\n");
		fprintf(stdout, "255\n");
		pattern_struct *pattern = lookup_table[k];
		// Write the matrix in PPM format
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				fprintf(stdout, "%d %d %d\n",
						pattern->pixels[i][j].red,
						pattern->pixels[i][j].green,
						pattern->pixels[i][j].blue
											);
			}
		}
	}
}

// The main function INIT_CONTUR that generates all the patterns
pattern_struct **initialize_contour_patterns(void)
{
	pattern_struct **lookup_table = malloc(16 * sizeof(pattern_struct *));
	if (!lookup_table) {
		printf("Eroare: Alocare memorie eșuată pentru tabela de cautare\n");
		return NULL;
	}
	for (int config = 0; config < 16; config++) {
		// Allocate memory for each individual pattern
		lookup_table[config] = malloc(sizeof(pattern_struct));
		if (!lookup_table[config]) {
			printf("Eroare: Alocare memorie eșuată %d", config);
			return NULL;
		}
		// Initialize the pattern
		generate_pattern(lookup_table, config);
	}
	return lookup_table;
}

// Function to free the memory of the image
void free_patterns(pattern_struct **lookup_table)
{
	// Free each individual pattern
	for (int config = 0; config < 16; config++) {
		free(lookup_table[config]);
	}
	// Free the lookup table itself
	free(lookup_table);
}
