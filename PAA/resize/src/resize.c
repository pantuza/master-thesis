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
        const PPMImage *image, int width, int height)
{
    PPMImage temp = image_new_from(image);

    if (!resize_pre_conditions(image, width, height))
        return temp;

    int *path, step;
    Color color;

    if (width)
    {
        color_init(&color, width);
        path = malloc(temp.height * sizeof(int));
        step = width;
        while(step--)
        {
            shortest_path_function(&temp, path);
            image_draw_path(image, &temp, path, &color);
            image_remove_path(&temp, path);
        };

        free(path);
    }

    if (height)
    {
        color_init(&color, height);
        PPMImage temp2 = image_new_transposed(&temp);
        path = malloc(temp2.height * sizeof(int));
        step = height;
        while(step--)
        {
            shortest_path_function(&temp2, path);
            image_draw_tpath(image, &temp2, path, &color);
            image_remove_path(&temp2, path);
        };
        image_copy_transposed(&temp, &temp2);
        free(path);
        image_free(&temp2);
    }

    return temp;
}

void print_path (int *p, int n, int c, int d, int l)
{
    fprintf(stderr, "[%3d(%d/%d)#%d] ", c, d, n, l);
    for (int i = 0; i < n; i++)
        fprintf(stderr, "%4d ", p[i]);
    fprintf(stderr, "\n");
}

int diff(int *p1, int*p2, int n)
{
    int different = 0;
    for (int i = 0; i < n; i++)
        if (p1[i] != p2[i])
        {
            p2[i] = -p2[i];
            different++;
        }
    return different;
}

/**
 * Compare results of different shortest path algorithms
 */
PPMImage image_resize_compare(ShortestPath function1, ShortestPath function2,
        const PPMImage *image, int width, int height)
{
    PPMImage temp = image_new_from(image);

    if (!resize_pre_conditions(image, width, height))
        return temp;

    int *path1, *path2, step, cnt, dif;
    Color color;

    if (width)
    {
        color_init(&color, width);
        path1 = calloc(temp.height, sizeof(int));
        path2 = calloc(temp.height, sizeof(int));
        step = width;
        cnt = 0;
        while(step--)
        {
            function1(&temp, path1);
            function2(&temp, path2);
            cnt++;
            if ((dif = diff(path1, path2, temp.height)))
            {
                print_path(path1, temp.height, cnt, dif, step);
                print_path(path2, temp.height, cnt, dif, step);
                image_draw_path(image, &temp, path1, &color);
            }
            image_remove_path(&temp, path1);
        };

        free(path1);
        free(path2);
    }

    if (height)
    {
        color_init(&color, height);
        PPMImage temp2 = image_new_transposed(&temp);
        path1 = calloc(temp.height, sizeof(int));
        path2 = calloc(temp.height, sizeof(int));
        step = height;
        cnt = 0;
        while(step--)
        {
            function1(&temp2, path1);
            function2(&temp2, path2);
            cnt++;
            if ((dif = diff(path1, path2, temp.height)))
            {
                print_path(path1, temp.height, cnt, dif, step);
                print_path(path2, temp.height, cnt, dif, step);
                image_draw_path(image, &temp, path1, &color);
            }
            image_remove_path(&temp2, path1);
        };
        image_copy_transposed(&temp, &temp2);
        free(path1);
        free(path2);
        image_free(&temp2);
    }

    return temp;
}

