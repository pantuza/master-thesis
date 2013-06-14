#include "dynamic.h"


void build_store(SeamsStore *seams, int width, int height)
{

    seams->size = width;
    seams->store = malloc(width * sizeof(int *));
    
    for(int i = 0; i < width; i++)
        seams->store[i] = malloc(height * sizeof(Seam));
}


void min_energy_upper(PPMImage *image, int x, int y, SeamsStore *seams)
{
    // left side
    if(!x)
    {
        if(image->pixels[0][y-1].energy < image->pixels[1][y-1].energy)
        {
            seams->store[x][y].dx = 0;
            seams->store[x][y].total = image->pixels[0][y].energy + image->pixels[0][y-1].energy;
        }
        else
        {
            seams->store[x][y].dx = 1;
            seams->store[x][y].total = image->pixels[0][y].energy + image->pixels[1][y-1].energy;
        }
    }
    // right side
    else if(x == (image->width - 1))
    {
        if(image->pixels[x-1][y-1].energy < image->pixels[x][y-1].energy)
        {
            seams->store[x][y].dx = -1;
            seams->store[x][y].total = image->pixels[x][y].energy + image->pixels[x-1][y-1].energy;
        }
        else
        {
            seams->store[x][y].dx = 0;
            seams->store[x][y].total = image->pixels[x][y].energy + image->pixels[x][y-1].energy;
        }
    }
    // middle
    else
    {
        if(image->pixels[x-1][y-1].energy < image->pixels[x][y-1].energy)
        {
            if(image->pixels[x-1][y-1].energy < image->pixels[x+1][y-1].energy)
            {
                seams->store[x][y].dx = -1;
                seams->store[x][y].total = image->pixels[x][y].energy + image->pixels[x-1][y-1].energy;
            }
            else
            {
                seams->store[x][y].dx = 1;
                seams->store[x][y].total = image->pixels[x][y].energy + image->pixels[x+1][y-1].energy;
            }
        }
        else
        {
            if(image->pixels[x][y-1].energy < image->pixels[x+1][y-1].energy)
            {
                seams->store[x][y].dx = 0;
                seams->store[x][y].total = image->pixels[x][y].energy + image->pixels[x][y-1].energy;
            }
            else
            {
                seams->store[x][y].dx = 1;
                seams->store[x][y].total = image->pixels[x][y].energy + image->pixels[x+1][y-1].energy;
            }
        }
    }
    //printf("dx: %d\ttotal: %.2f\n", seams->store[x][y].dx, seams->store[x][y].total);
}



void seam_carving(PPMImage *image)
{
    SeamsStore seams;
    build_store(&seams, image->width, image->height);

    int x, y;
    printf("image width: %d\n", image->width);
    for(x = 0; x < image->width; x++)
    {
        for(y = 1; y < image->height; y++)
        {
          //  printf("x: %d\t y: %d\t energy: %.2lf\t", x, y, image->pixels[x][y].energy);
            min_energy_upper(image, x, y, &seams);
        }
    }


    /* Get init of the shortest path */
    y = image->height - 1;
    Energy min = seams.store[0][y].total;
    int minx = 0;
    for(x = 1; x < image->width; x++)
        if(seams.store[x][y].total < min)
        {
            min = seams.store[x][y].total;
            minx = x;
        }

    x = minx;
    for(; y >= 0; y--)
    {
        image->pixels[x][y].R = 255;
        image->pixels[x][y].G = 0;
        image->pixels[x][y].B = 0;
        image->pixels[x][y].energy = image->energy + 1;
        //printf("x: %d y %d dx: %d\n", x, y, seams.store[x][y].dx);
        x += seams.store[x][y].dx;
    }

}


void dynamic_resize(PPMImage *image, int width, int height)
{
    printf("dynamic programming...\n");
    while(width--)
        seam_carving(image);
}
