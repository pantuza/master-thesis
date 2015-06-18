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
   int first_dimension = weight->height * sizeof(int *);
   int second_dimension = weight->width * sizeof(Energy);

   weight->matrix = malloc(first_dimension);
   for(int i = 0; i < weight->height; i++)
       weight->matrix[i] = malloc(second_dimension);
}

/**
 * Deallocate the storage to the matrix
 */
void matrix_deallocation(WeightMatrix *weight)
{
   for(int i = 0; i < weight->height; i++)
       free(weight->matrix[i]);
   free(weight->matrix);
}

/**
 * Make a default  weights matrices
 * allocate the matrices and fill data on it
 */
void load_defaul(Sobel *weights)
{
    /* Second Sobel Matrix Gx */
    /* Firt Sobel Matrix Gx */
    weights->Mx.height = 3;
    weights->Mx.width = 3;
    matrix_allocation(&(weights->Mx));
    weights->Mx.matrix[0][0] =  1;
    weights->Mx.matrix[1][0] =  2;
    weights->Mx.matrix[2][0] =  1;
    weights->Mx.matrix[0][1] =  0;
    weights->Mx.matrix[1][1] =  0;
    weights->Mx.matrix[2][1] =  0;
    weights->Mx.matrix[0][2] = -1;
    weights->Mx.matrix[1][2] = -2;
    weights->Mx.matrix[2][2] = -1;

    /* Set Sobel Matrix Gy */
    weights->My.height = 3;
    weights->My.width = 3;
    matrix_allocation(&(weights->My));
    weights->My.matrix[0][0] =  1;
    weights->My.matrix[0][1] =  2;
    weights->My.matrix[0][2] =  1;
    weights->My.matrix[1][0] =  0;
    weights->My.matrix[1][1] =  0;
    weights->My.matrix[1][2] =  0;
    weights->My.matrix[2][0] = -1;
    weights->My.matrix[2][1] = -2;
    weights->My.matrix[2][2] = -1;

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
    for(int i = 0; i < weight->height; i++)
        for(int j = 0; j < weight->width; j++)
        {
            fscanf(file, ENERGY_FORMAT, &(weight->matrix[i][j]));
            checkTotalWeight += weight->matrix[i][j];
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
    read_dimension(weights_file, &(weights->Mx));
    matrix_allocation(&(weights->Mx));
    fill_weights_data(weights_file, &(weights->Mx));

    /* Second Sobel Matrix Gx */
    read_dimension(weights_file, &(weights->My));
    matrix_allocation(&(weights->My));
    fill_weights_data(weights_file, &(weights->My));

    file_close(weights_file);
}

/**
 * deallocate the matrices
 */
void free_matrices(Sobel *weights)
{
    matrix_deallocation(&(weights->Mx));
    matrix_deallocation(&(weights->My));
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
#define luminosity(p) (R_SENS*(p).R + G_SENS*(p).G + B_SENS*(p).B)
//}


/**
 * Calculate gradient (Sobel's approximation) of a (x, y) pixel
 */
Energy gradient(PPMImage *image, int x, int y, WeightMatrix *M)
{
    int dx = x - (M->height / 2);
    int dy = y - (M->width / 2);

    int cx, cy;
    Energy g = 0;

    /* G({x|y}) gradient */
    for(int i = 0; i < M->height; i++)
        for(int j = 0; j < M->width; j++)
        {
            // mirroring pixel at leftmost
            cy = abs(dy+j);
            // mirroring pixel at top
            cx = abs(dx+i);
            // mirroring pixel at rightmost
            if (cy >= image->height)
                cy = 2*image->height - cy - 1;
            // mirroring pixel at bottom
            if (cx >= image->width)
                cx = 2*image->width - cx - 1;

            g += M->matrix[i][j] * luminosity(image->pixels[cx][cy]);
        }
    return g;
}


/**
 * Sobel Calculation 
 */
void sobel_calc(PPMImage *image, int x, int y, Sobel *sobel)
{
    /* Coordinates of the gradient vector */
    Energy gx, gy, energy;
    gx = gradient(image, x, y, &(sobel->Mx));
    gy = gradient(image, x, y, &(sobel->My));
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
            sobel_calc(image, x, y, &sobel);
    free_matrices(&sobel);
}
