/*
 * dynamic.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#include <float.h>

#include "debug.h"
#include "color.h"
#include "ppm.h"

typedef struct
{
    Energy total;
    int dx;
} PixelEnergySum;


typedef PixelEnergySum ** EnergySum;


EnergySum allocate_energy_sum(int width, int height)
{

    int first_dimension = width * sizeof(PixelEnergySum *);
    int second_dimension = (width * height) * sizeof(PixelEnergySum);

    // Allocate consecutive memory
    EnergySum sum = malloc(first_dimension + second_dimension);
    if (sum == NULL)
    {
        fprintf(stderr, "Dynamic programming allocation failure.\n");
        exit(EXIT_FAILURE);
    }

    sum[0] = (PixelEnergySum *)(sum + width);
    for(int i = 1; i < width; i++)
        sum[i] = (PixelEnergySum *)(sum[i-1] + height);

    return sum;
}

void min_energy_upper(PPMImage *image, int x, int y, EnergySum sum)
{
    // left side
    if(!x)
    {
        if(sum[0][y-1].total < sum[1][y-1].total)
        {
            sum[0][y].dx = 0;
            sum[0][y].total = sum[0][y-1].total;
        }
        else
        {
            sum[0][y].dx = 1;
            sum[0][y].total = sum[1][y-1].total;
        }
    }
    // right side
    else if(x == (image->width - 1))
    {
        if(sum[x-1][y-1].total < sum[x][y-1].total)
        {
            sum[x][y].dx = -1;
            sum[x][y].total = sum[x-1][y-1].total;
        }
        else
        {
            sum[x][y].dx = 0;
            sum[x][y].total = sum[x][y-1].total;
        }
    }
    // middle
    else
    {
        if(sum[x-1][y-1].total < sum[x][y-1].total)
        {
            if(sum[x-1][y-1].total < sum[x+1][y-1].total)
            {
                sum[x][y].dx = -1;
                sum[x][y].total = sum[x-1][y-1].total;
            }
            else
            {
                sum[x][y].dx = 1;
                sum[x][y].total = sum[x+1][y-1].total;
            }
        }
        else
        {
            if(sum[x][y-1].total < sum[x+1][y-1].total)
            {
                sum[x][y].dx = 0;
                sum[x][y].total = sum[x][y-1].total;
            }
            else
            {
                sum[x][y].dx = 1;
                sum[x][y].total = sum[x+1][y-1].total;
            }
        }
    }
    sum[x][y].total += image->pixels[x][y].energy;
}

int dp_find_shortest_path(PPMImage *image, const EnergySum sum)
{
    // Get init of the shortest path
    int y = image->height - 1;
    Energy min;
    min = ENERGY_MAX;
    int minx = 0;
    for(int x = 0; x < image->width; x++)
        if(sum[x][y].total < min)
        {
            min = sum[x][y].total;
            minx = x;
        }
    return minx;
}

void dp_make_shortest_path(int * path, PPMImage *image,
        const EnergySum sum, int minx)
{
    int y = image->height - 1;
    path[y] = minx;
    do
    {
        path[y-1] = path[y] + sum[path[y]][y].dx;
    } while(--y);
}

void dp_start(PPMImage *image, const EnergySum sum)
{
    for(int x = 0; x < image->width; x++)
        sum[x][0].total = image->pixels[x][0].energy;
}


void dp_shortest_path(PPMImage *image, int *path)
{
    int x, y;
    EnergySum sum = allocate_energy_sum(image->width, image->height);
    dp_start(image, sum);
    for(y = 1; y < image->height; y++)
        for(x = 0; x < image->width; x++)
            min_energy_upper(image, x, y, sum);
    int minx = dp_find_shortest_path(image, sum);
    dp_make_shortest_path(path, image, sum, minx);
    free(sum);
}

