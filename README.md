// Copyright Traistaru Dragos-Andrei
# Marching-Squares

The solution is structured in three files:
  - main, which controls image processing operations based on user commands;
  - image_utils, contains core image processing functions, including reading, writing, resizing, 
  and freeing PPM images, as well as initializing and creating the lookup table with the patterns;
  - marching_squares, contains the logic for creating the binary grid and for applying the marching 
  squares algorithm;

After reading the image from the PPM file, the grid is computed in several steps.  
First, a luminosity matrix is created based on the image data. This matrix is then  
traversed in steps of `GRID_STEP`, and for each element, the average value with  
its neighbors is calculated and stored in the grid.  

Next, the computed grid is converted into a binary grid by comparing each element  
with a predefined threshold.  

Before applying the Marching Squares algorithm, a lookup table containing patterns  
is created. This table is implemented as an array of structures called  
`pattern_struct`. Each `pattern_struct` contains a `key` representing a  
configuration number and a 4x4 matrix defining the specific pattern for that key.  
Patterns are generated using the `generate_pattern` function, which converts the  
key into its binary form. Based on the number of black corners (binary 1s), the  
function determines the correct shape—such as a triangle, rectangle, trapezoid,  
or square—and generates the corresponding pattern.  

In the final step, the binary grid is used to create the output image. The grid is  
traversed, and for each element, the configuration number is determined by  
examining its neighbors (the square's corners). This configuration number serves  
as a key to retrieve the appropriate pattern from the lookup table. The  
`place_pattern_in_image` function is used to insert each 4x4 pattern into the  
final image at the corresponding position.  
