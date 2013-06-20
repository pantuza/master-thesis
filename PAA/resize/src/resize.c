/*
 * resize.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */

#include <stdio.h>

#include "resize.h"
#include "color.h"

int resize_pre_conditions(const PPMImage *image,
        const int width, const int height)
{
    if (width > image->width || height > image->height)
    {
        fprintf(stderr, "ERROR: Resize parameters too big for image.");
        return 0;
    }

    if (image->width < 2 || image->height < 2)
    {
        fprintf(stderr, "ERROR: Image size too small to resize.");
        return 0;
    }
    return 1;
}

/**
 * General resizing by different shortest path algorithms
 */
PPMImage image_resize(ShortestPath shortest_path_function,
        const PPMImage *image, int width, int height)
{
    PPMImage temp = image_new_from(image);

    if (!resize_pre_conditions(image, width, height))
        return temp;

    int *path, step;

    if (width)
    {
        path = malloc(temp.height * sizeof(int));
        step = width;
        while(step--)
        {
            shortest_path_function(&temp, path);
            image_remove_path(&temp, path);
        };

        free(path);
    }

    if (height)
    {
        PPMImage temp2 = image_new_transposed(&temp);
        path = malloc(temp2.height * sizeof(int));
        step = height;
        while(step--)
        {
            shortest_path_function(&temp2, path);
            image_remove_path(&temp2, path);
        };
        image_copy_transposed(&temp, &temp2);
        free(path);
        image_free(&temp2);
    }

    return temp;
}

/**
 * General resizing by different shortest path algorithms
 */
PPMImage image_resize_preview(ShortestPath shortest_path_function,
        PPMImage *image, int width, int height)
{
    PPMImage temp_x = image_new_from(image);

    if (!resize_pre_conditions(image, width, height))
        return temp_x;

    int *path, step;
    Color color;

    if (width)
    {
        RED(&color, width);
        path = malloc(temp_x.height * sizeof(int));
        step = width;
        while(step--)
        {
            shortest_path_function(&temp_x, path);
            image_draw_path(image, &temp_x, path, &color);
            image_remove_path(&temp_x, path);
        };

        free(path);
    }

    if (height)
    {
        BLUE(&color, height);
        PPMImage temp_y = image_new_transposed(&temp_x);
        PPMImage temp_t = image_new_transposed(image);
        path = malloc(temp_y.height * sizeof(int));
        step = height;
        while(step--)
        {
            shortest_path_function(&temp_y, path);
            image_draw_tpath(&temp_t, &temp_y, path, &color);
            image_remove_path(&temp_y, path);
        };
        image_copy_transposed(&temp_x, &temp_y);
        image_copy_transposed(image, &temp_t);
        free(path);
        image_free(&temp_t);
        image_free(&temp_y);
    }

    return temp_x;
}

