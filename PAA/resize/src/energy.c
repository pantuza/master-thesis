#include <math.h>

#include "energy.h"
#include "ppm.h"
#include "file.h"


void load_matrix(char *matrix)
{
    FILE *weights = openfile(matrix, READ_MODE);
    
    closefile(weights);
}


float luminosity(Pixel *pixel)
{
    return (R_SENS * pixel->R + G_SENS * pixel->G + B_SENS * pixel->B);
}


void sobel(Pixel *pixel)
{
//    float lumin = luminosity(pixel);

    // pixel->energy = energy;
}


void energise(PPMImage *image, char *matrix)
{
    load_matrix(matrix);

    for(int i = 0; i < image->width; i++)
        for(int j = 0; j < image->height; j++)
        {
            sobel(&image->pixels[i][j]);
        }
}
