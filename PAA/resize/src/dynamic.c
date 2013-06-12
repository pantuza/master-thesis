#include "dynamic.h"


void build_store(SeamsStore *seams, int width, int height)
{

    seams->size = width;
    seams->store = malloc(width * sizeof(int *));
    
    for(int i = 0; i < width; i++)
        seams->store[i] = malloc(height * sizeof(Seam));
}

void min_energy_upper(PPMImage *image, int i, int j, SeamsStore *seams)
{
    // left side
    if(!(j % image->height))
    {
        printf("left\n");
        
    }
    // right side
    else if((j % image->height) == (image->height - 1))
    {
        printf("right\n");
    }
    // middle
    else
    {
        printf("middle\n");
    }
}



void seam_carving(PPMImage *image)
{
    SeamsStore seams;
    build_store(&seams, image->width, image->height);

    printf("image width: %d\n", image->width);
    for(int i = 1; i < image->width; i++)
    {
        for(int j = 0; j < image->height; j++)
        {
            printf("i: %d\t j: %d\t energy: %.2lf\t", i, j, image->pixels[j][i].energy);
            min_energy_upper(image, i, j, &seams);
        }
    }
}


void dynamic_resize(PPMImage *image, int width, int height)
{
    printf("dynamic programming...\n");
    seam_carving(image);
}
