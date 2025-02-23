#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#define SIZE 4
#define MAX_COLOR_VALUE 255
#define RESIZE_FACTOR 4
#define GRID_STEP 4
#define THRESHOLD 200

typedef struct {
	int red;
	int green;
	int blue;
} pixel_struct; // Pixel with RGB channels

typedef struct {
	int width;
	int height;
	int max_color;
	pixel_struct **pixels; // Matrix of pixels that form the image
} image_struct;

typedef struct {
	int key; // Configuration key (0-15)
	pixel_struct pixels[SIZE][SIZE]; // 4x4 grid of pixels
} pattern_struct;

image_struct *read_ppm(void);
void write_ppm(image_struct *image);
image_struct *resize_image(image_struct *image, int factor);
void free_image(image_struct *image);
pixel_struct create_pixel(int value);
pattern_struct **initialize_contour_patterns(void);
void free_patterns(pattern_struct **lookup_table);
void print_contur(pattern_struct **lookup_table);

#endif
