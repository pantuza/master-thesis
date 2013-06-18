/*
 * energy.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "energy.h"
#include "ppm.h"
#include "file.h"
#include "debug.h"


/**
 * Allocate the storage to the matrix
 */
void matrix_allocation(WeightMatrix *weight)
{
   int first_dimension = weight->width * sizeof(int *);
   int second_dimension = weight->height * sizeof(Energy);

   weight->matrix = malloc(first_dimension);
   for(int x = 0; x < weight->width; x++)
       weight->matrix[x] = malloc(second_dimension);
}

/**
 * Deallocate the storage to the matrix
 */
void matrix_deallocation(WeightMatrix *weight)
{
   for(int x = 0; x < weight->width; x++)
       free(weight->matrix[x]);
   free(weight->matrix);
}

/**
 * Make a default  weights matrices
 * allocate the matrices and fill data on it
 */
void load_defaul(Sobel *weights)
{
    /* Firt Sobel Matrix Gx */
    weights->Gx.height = 3;
    weights->Gx.width = 3;
    matrix_allocation(&(weights->Gx));
    weights->Gx.matrix[0][0] =  1;
    weights->Gx.matrix[0][1] =  2;
    weights->Gx.matrix[0][2] =  1;
    weights->Gx.matrix[1][0] =  0;
    weights->Gx.matrix[1][1] =  0;
    weights->Gx.matrix[1][2] =  0;
    weights->Gx.matrix[2][0] = -1;
    weights->Gx.matrix[2][1] = -2;
    weights->Gx.matrix[2][2] = -1;

    /* Second Sobel Matrix Gx */
    /* Firt Sobel Matrix Gx */
    weights->Gy.height = 3;
    weights->Gy.width = 3;
    matrix_allocation(&(weights->Gy));
    weights->Gy.matrix[0][0] =  1;
    weights->Gy.matrix[1][0] =  2;
    weights->Gy.matrix[2][0] =  1;
    weights->Gy.matrix[0][1] =  0;
    weights->Gy.matrix[1][1] =  0;
    weights->Gy.matrix[2][1] =  0;
    weights->Gy.matrix[0][2] = -1;
    weights->Gy.matrix[1][2] = -2;
    weights->Gy.matrix[2][2] = -1;
}

/**
 * Reads from file the dimensions of a weight matrix
 */
void read_dimension(FILE *file, WeightMatrix *matrix)
{
    fscanf(file, "%d %d", &matrix->width, &matrix->height);
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
            fscanf(file, ENERGY_FORMAT, &(weight->matrix[x][y]));
            checkTotalWeight += weight->matrix[x][y];
        }
    ASSERT_FALSE(checkTotalWeight,
        fprintf(stderr, "ASSERT: total weight of Sobel matrix is not zero!"));
}

/**
 * Reads the dimension of the weights matrices
 * allocate the matrices and fill data on it
 */
void load_matrices(Sobel *weights, char *matrix)
{
    FILE *weights_file = file_open(matrix, READ_MODE);

    /* Firt Sobel Matrix Gx */
    read_dimension(weights_file, &(weights->Gx));
    matrix_allocation(&(weights->Gx));
    fill_weights_data(weights_file, &(weights->Gx));

    /* Second Sobel Matrix Gx */
    read_dimension(weights_file, &(weights->Gy));
    matrix_allocation(&(weights->Gy));
    fill_weights_data(weights_file, &(weights->Gy));

    file_close(weights_file);
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
 * Calculate gradient (Sobel's approximation) of a (x, y) pixel
 */
Energy gradient(PPMImage *image, int x, int y, WeightMatrix *G)
{
    int dy = x - (G->height / 2);
    int dx = y - (G->width / 2);

    int mx, my;
    Energy g = 0;

    /* G({x|y}) gradient */
    for(int yi = 0; yi < G->height; yi++)
        for(int xi = 0; xi < G->width; xi++)
        {
            // mirroring pixel at leftmost
            my = abs(dy+yi);
            // mirroring pixel at top
            mx = abs(dx+xi);
            // mirroring pixel at rightmost
            if (my >= image->height)
                my = 2*image->height - my - 1;
            // mirroring pixel at bottom
            if (mx >= image->width)
                mx = 2*image->width - mx - 1;

            g += G->matrix[yi][xi] * luminosity(image->pixels[mx][my]);
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
}


/**
 * Calculates the energy for each pixel in the image 
 */
void energise(PPMImage *image, char *matrix)
{
    Sobel sobel;

    if (matrix == NULL)
        load_defaul(&sobel);
    else
        load_matrices(&sobel, matrix);

    for(int y = 0; y < image->height; y++)
        for(int x = 0; x < image->width; x++)
            sobel_calc(image, y, x, &sobel);
    free_matrices(&sobel);
}
