#include <math.h>

#include "energy.h"
#include "ppm.h"
#include "file.h"


/**
 * Reads from file the dimensions of a weight matrix
 */
void read_dimension(FILE *file, WeightMatrix *matrix)
{
    char buffer[32];
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d %d", &matrix->width, &matrix->height);
}


/**
 * Allocate the storage to the matrix
 */
void matrix_allocation(WeightMatrix *weights)
{
   int first_dimension = weights->width * sizeof(int *);
   int second_dimension = (weights->width * weights->height) * sizeof(float);

   weights->matrix = (float **) malloc(first_dimension + second_dimension);
}


/**
 * Reads from file the values to fill the weight matrix
 */
void fill_weights_data(FILE *file, WeightMatrix *weight)
{
    float number;

    for(int i = 0; i < weight->width; i++)
        for(int j = 0; j < weight->height; j++)
        {
            fscanf(file, "%f", &number);
            if(number == EOF)
            {
                fprintf(stderr, "Malformed file");
                exit(EXIT_FAILURE);
            }
            weight->matrix[i][j] = number;
        }
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

    Sobel weights = load_matrices(matrix);

    for(int i = 0; i < image->width; i++)
        for(int j = 0; j < image->height; j++)
            sobel(image->pixels, i, j, &weights);
}
