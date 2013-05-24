#include <math.h>
#include <string.h>

#include "energy.h"
#include "ppm.h"
#include "file.h"


/**
 * Reads from file the dimensions of a weight matrix
 */
void read_dimension(FILE *file, WeightMatrix *matrix)
{
    fscanf(file, "\n");
    char buffer[64];
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d %d", &matrix->width, &matrix->height);
}


/**
 * Allocate the storage to the matrix
 */
void matrix_allocation(WeightMatrix *weight)
{
   int first_dimension = weight->width * sizeof(int *);
   int second_dimension = weight->height * sizeof(float);

   weight->matrix = malloc(first_dimension);
   for(int i = 0; i < weight->width; i++)
       weight->matrix[i] = malloc(second_dimension);
}


/**
 * Reads from file the values to fill the weight matrix
 */
void fill_weights_data(FILE *file, WeightMatrix *weight)
{
   fprintf(stdout, "w: %d h: %d\n", weight->width, weight->height);
    float number;
    int scan;
    for(int i = 0; i < weight->width; i++)
    {
        for(int j = 0; j < weight->height && scan != EOF; j++)
        {
            scan = fscanf(file, "%f", &number);
            weight->matrix[i][j] = number;
            fprintf(stdout, "n: %.1f i: %d j:%d\n", weight->matrix[i][j], i , j);
        }
    }
}


/**
 * Reads the dimension of the weights matrices
 * allocate the matrices and fill data on it
 */
Sobel load_matrices(char *matrix)
{
    fprintf(stdout, "matrix: %s\n", matrix);
    FILE *weights_file = openfile(matrix, READ_MODE);
    Sobel weights;

    /* Firt Sobel Matrix Gx */
    read_dimension(weights_file, &weights.Gx);
    matrix_allocation(&weights.Gx);
    fill_weights_data(weights_file, &weights.Gx);

    /* Second Sobel Matrix Gx */
    read_dimension(weights_file, &weights.Gy);
    matrix_allocation(&weights.Gy);
    fill_weights_data(weights_file, &weights.Gy);

    closefile(weights_file);
    return weights;
}


/**
 * Calculates the absolute value of the gradiente vector
 */
float energy(float gx, float gy)
{
    return sqrt((pow(gx, 2) + pow(gy, 2)));
}


/**
 * returns the luminosity intensity of a given pixel
 * based on human eyes sensibility of lights
 */
float luminosity(Pixel *pixel)
{
    return (R_SENS * pixel->R + G_SENS * pixel->G + B_SENS * pixel->B);
}


/**
 * Calculates the gradient of a pixel
 */
void gradient(Pixel **pixels, int i, int j, Sobel *sobel, float *gx, float *gy)
{
    int k = i - (sobel->Gx.width - 2);
    int l = j - (sobel->Gy.height - 2);

    /* Gx gradient */ 
    for(int m = 0; m < sobel->Gx.width; m++)
        for(int n = 0; n < sobel->Gx.height; n++)
            *gx += sobel->Gx.matrix[m][n] * luminosity(&pixels[k+m][l+n]);

    /* Gy gradient */
     for(int m = 0; m < sobel->Gy.width; m++)
        for(int n = 0; n < sobel->Gy.height; n++)
            *gy += sobel->Gy.matrix[m][n] * luminosity(&pixels[k+m][l+n]);
           
}


/**
 * Sobel Calculation 
 */
void sobel(Pixel **pixels, int i, int j, Sobel *sobel)
{
    /* Coordinates of the gradient vector */
    float gx, gy; 
    gradient(pixels, i, j, sobel, &gx, &gy);
    pixels[i][j].energy = energy(gx, gy);    
}


/**
 * Calculates the energy for each pixel in the image 
 */
void energise(PPMImage *image, char *matrix)
{
    fprintf(stdout, "here: %s\n", matrix);

    Sobel weights = load_matrices(matrix);

    for(int i = 0; i < image->width; i++)
        for(int j = 0; j < image->height; j++)
            sobel(image->pixels, i, j, &weights);
}
