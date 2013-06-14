#include <math.h>
#include <string.h>
#include <stdio.h>

#include "energy.h"
#include "ppm.h"
#include "file.h"
#include "debug.h"

/**
 * Reads from file the dimensions of a weight matrix
 */
void read_dimension(FILE *file, WeightMatrix *matrix)
{
    fscanf(file, "%d %d", &matrix->width, &matrix->height);
}


/**
 * Allocate the storage to the matrix
 */
void matrix_allocation(WeightMatrix *weight)
{
   int first_dimension = weight->height * sizeof(int *);
   int second_dimension = weight->width * sizeof(Energy);

   weight->matrix = malloc(first_dimension);
   for(int y = 0; y < weight->height; y++)
       weight->matrix[y] = malloc(second_dimension);
}

/**
 * Deallocate the storage to the matrix
 */
void matrix_deallocation(WeightMatrix *weight)
{
   for(int y = 0; y < weight->height; y++)
       free(weight->matrix[y]);
   free(weight->matrix);
}


/**
 * Reads from file the values to fill the weight matrix
 */
void fill_weights_data(FILE *file, WeightMatrix *weight)
{
    Energy checkTotalWeight = 0;
    for(int y = 0; y < weight->height; y++)
        for(int x = 0; x < weight->width; x++)
        {
            fscanf(file, ENERGY_FORMAT, &(weight->matrix[y][x]));
            checkTotalWeight += weight->matrix[y][x];
        }
    ASSERT_FALSE(checkTotalWeight,
        fprintf(stderr, "ASSERT: total weight of Sobel matrix is not zero!"));
}

/**
 * Reads the dimension of the weights matrices
 * allocate the matrices and fill data on it
 */
Sobel load_matrices(char *matrix)
{
    FILE *weights_file = openfile(matrix, READ_MODE);
    Sobel weights;

    /* Firt Sobel Matrix Gx */
    read_dimension(weights_file, &(weights.Gx));
    matrix_allocation(&(weights.Gx));
    fill_weights_data(weights_file, &(weights.Gx));

    /* Second Sobel Matrix Gx */
    read_dimension(weights_file, &(weights.Gy));
    matrix_allocation(&(weights.Gy));
    fill_weights_data(weights_file, &(weights.Gy));

    closefile(weights_file);
    return weights;
}


/**
 * deallocate the matrices
 */
void free_matrices(Sobel *weights)
{
    matrix_deallocation(&(weights->Gx));
    matrix_deallocation(&(weights->Gy));
}

/**
 * Calculates the absolute value of the gradiente vector
 */
//Energy energy(Energy gx, Energy gy)
//{
#define energy(gx, gy) (sqrt((pow(gx, 2) + pow(gy, 2))))
//}


/**
 * returns the luminosity intensity of a given pixel
 * based on human eyes sensibility of lights
 */
//Energy luminosity(Pixel *pixel)
//{
#define luminosity(p) (R_SENS * (p).R + G_SENS * (p).G + B_SENS * (p).B)
//}


/**
 * Calculates the gradient of a pixel
 */
Energy gradient(PPMImage *image, int i, int j, WeightMatrix *G)
{
    int k = i - (G->height / 2);
    int l = j - (G->width / 2);

    int x, y;
    Energy g = 0;

    /* G(xy) gradient */
    for(int m = 0; m < G->height; m++)
        for(int n = 0; n < G->width; n++)
        {
            y = abs(k+m);
            x = abs(l+n);

            if (y >= image->height)
                y = 2*image->height - y - 1;
            if (x >= image->width)
                x = 2*image->width - x - 1;

            g += G->matrix[m][n] * luminosity(image->pixels[x][y]);
        }
    return g;
}


/**
 * Sobel Calculation 
 */
void sobel_calc(PPMImage *image, int y, int x, Sobel *sobel)
{
    /* Coordinates of the gradient vector */
    Energy gx, gy, energy;
    gx = gradient(image, y, x, &(sobel->Gx));
    gy = gradient(image, y, x, &(sobel->Gy));
    energy = energy(gx, gy);
    image->pixels[x][y].energy = energy;
    if (image->energy < energy)
        image->energy = energy;
}


/**
 * Calculates the energy for each pixel in the image 
 */
void energise(PPMImage *image, char *matrix)
{
    image->energy = 0;
    Sobel sobel = load_matrices(matrix);
    for(int y = 0; y < image->height; y++)
        for(int x = 0; x < image->width; x++)
            sobel_calc(image, y, x, &sobel);
    free_matrices(&sobel);
}
