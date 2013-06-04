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
    fscanf(file, "%d %d", &matrix->width, &matrix->height);
}


/**
 * Allocate the storage to the matrix
 */
void matrix_allocation(WeightMatrix *weight)
{
   int first_dimension = weight->width * sizeof(int *);
   int second_dimension = weight->height * sizeof(double);

   weight->matrix = malloc(first_dimension);
   for(int i = 0; i < weight->width; i++)
       weight->matrix[i] = malloc(second_dimension);
}


/**
 * Reads from file the values to fill the weight matrix
 */
void fill_weights_data(FILE *file, WeightMatrix *weight)
{
    for(int i = 0; i < weight->width; i++)
    {
        fscanf(file, "\n");
        for(int j = 0; j < weight->height; j++)
            fscanf(file, "%lf", &(weight->matrix[i][j]));
            
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
    read_dimension(weights_file, &(weights.Gx));
    matrix_allocation(&weights.Gx);
    fill_weights_data(weights_file, &(weights.Gx));

    /* Second Sobel Matrix Gx */
    read_dimension(weights_file, &(weights.Gy));
    matrix_allocation(&weights.Gy);
    fill_weights_data(weights_file, &(weights.Gy));

    closefile(weights_file);
    return weights;
}


/**
 * Calculates the absolute value of the gradiente vector
 */
//double energy(double gx, double gy)
//{
#define energy(gx, gy) (sqrt((pow(gx, 2) + pow(gy, 2))))
//}


/**
 * returns the luminosity intensity of a given pixel
 * based on human eyes sensibility of lights
 */
//double luminosity(Pixel *pixel)
//{
#define luminosity(p) (R_SENS * (p).R + G_SENS * (p).G + B_SENS * (p).B)
//}


/**
 * Calculates the gradient of a pixel
 */
double gradient(PPMImage *image, int i, int j, WeightMatrix *G)
{
    int k = i - (G->width / 2);
    int l = j - (G->height / 2);

    int x, y;
    double g = 0;

    printf("-- xy --\n");
    printf("w: %d h: %d\n", G->width, G->height);
    /* Gx gradient */ 
    for(int m = 0; m < G->width; m++)
        for(int n = 0; n < G->height; n++)
        {
            x = abs(k+m);
            y = abs(l+n);

            if (x >= image->width)
                x = 2*image->width - x - 1;
            if (y >= image->height)
                y = 2*image->height - y - 1;

            g += G->matrix[m][n] * luminosity(image->pixels[x][y]);
            printf("x: %d\ty: %d\tm: %d\tn: %d\tpx: %d,%d,%d\tlumi: %1.lf \tsobel: %1.lf \tg: %1.lf\n", x, y, m, n, image->pixels[x][y].R, image->pixels[x][y].G, image->pixels[x][y].B, luminosity(image->pixels[x][y]),G->matrix[m][n], g);
        }
    return g;
}


/**
 * Sobel Calculation 
 */
void sobel_calc(PPMImage *image, int i, int j, Sobel *sobel)
{
    /* Coordinates of the gradient vector */
    double gx, gy;
    gx = gradient(image, i, j, &(sobel->Gx));
    gy = gradient(image, i, j, &(sobel->Gy));
    printf("gx: %1.lf\t gy: %1.lf\n", gx, gy);
    image->pixels[i][j].energy = energy(gx, gy);
    fprintf(stdout, "pixel[%d][%d]: %1.lf\n", i, j, image->pixels[i][j].energy);
}


/**
 * Calculates the energy for each pixel in the image 
 */
void energise(PPMImage *image, char *matrix)
{
    fprintf(stdout, "here: %s\n", matrix);

    Sobel sobel = load_matrices(matrix);

    for(int i = 0; i < image->width; i++)
        for(int j = 0; j < image->height; j++)
            sobel_calc(image, i, j, &sobel);
            //printf("px[%d][%d]: %d,%d,%d\n", i, j, image->pixels[i][j].R,  image->pixels[i][j].G, image->pixels[i][j].B);

}
