/*
 * test.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#include <stdio.h>

#include "test.h"

int diff(int *p1, int*p2, int n)
{
    int different = 0;
    for (int i = 0; i < n; i++)
        if (p1[i] != p2[i])
            different++;
    return different;
}

void print_paths(char d, int i, int *p, int *q, int n)
{
    fprintf(stderr, "[%c:%d,path1]", d, i);
    for (int i = 0; i < n; i++)
        fprintf(stderr, "%4d ", p[i]);
    fprintf(stderr, "\n[%c:%d,path2]", d, i);
    for (int i = 0; i < n; i++)
        fprintf(stderr, "%4d ", (q[i] == p[i]? q[i]: -q[i]));
    fprintf(stderr, "\n");
}

Energy energy_path (int *p, PPMImage *img)
{
    Energy e = 0;
    for (int i = 0; i < img->height; i++)
        e += img->pixels[p[i]][i].energy;
    return e;
}

int diff_image(PPMImage *img1, PPMImage *img2)
{
    int dife = 0, difc = 0, difo = 0;
    Pixel p1, p2;
    for(int y = 0; y < img1->height; y++)
        for(int x = 0; x < img1->width; x++) {
            p1 = img1->pixels[x][y];
            p2 = img2->pixels[x][y];
            if ((p1.R != p2.R) || (p1.G != p2.G) || (p1.B != p2.B))
                fprintf(stderr, "#%d[%d,%d]:p1.color(%d,%d, %d) != p2.color(%d,%d, %d)\n",
                        ++difc, x,y,p1.R,p1.G,p1.B,p2.R,p2.G,p2.B);
            if (p1.energy != p2.energy)
                fprintf(stderr, "$%d[%d,%d]:p1.e(%f) != p2.e(%f)\n",
                        ++dife,x,y,p1.energy,p2.energy);
            if ((p1.x != p2.x) || (p1.y != p2.y))
                fprintf(stderr, "$%d[%d,%d]:p1.o(%d,%d) != p2.o(%d,%d)\n",
                        ++difo,x,y,p1.x,p1.y,p2.x,p2.y);
        }
    return dife + difc + difo;
}

int diff_energy(char d, int i, int *p,
        PPMImage *img_p, int *q, PPMImage *img_q, int printEquals)
{
    Energy ep = energy_path(p, img_p);
    Energy eq = energy_path(q, img_q);
    int dif_e = 0;
    if (ep  > eq) {
        dif_e = 1;
        fprintf(stderr, "[%c:%d] Total energy 1:%f > 2:%f!\n", d, i, ep, eq);
    }else
        if (ep  < eq) {
            dif_e = 1;
            fprintf(stderr, "[%c:%d] Total energy 1:%f < 2:%f!\n", d, i, ep, eq);
        } else
            if (printEquals)
                fprintf(stderr, "[%c:%d] Total energy equals %f!\n", d, i, ep);
    return dif_e;
}

int differ(char d, int i, int *p, PPMImage *img_p, int *q, PPMImage *img_q)
{
    if ((img_p->height != img_q->height) || (img_p->width != img_q->width)) {
        fprintf(stderr, "[%c:%d] Image dimension fail.\n", d, i);
        return 1;
    }

    int dif_path, dif_img;
    if ((dif_path = diff(p, q, img_p->height)))
        print_paths(d, i, p, q, img_q->height);

    int dif_e = diff_energy(d, i, p, img_p, q, img_q, 0);
    if (dif_path && !dif_e) {
        fprintf(stderr, "[%c:%d,p2->I1 x p1->I2]", d, i);
        dif_e += diff_energy(d, i, q, img_p, p, img_q, 1);
        fprintf(stderr, "[%c:%d,p1->I2 x p2->I1]", d, i);
        dif_e += diff_energy(d, i, p, img_q, q, img_p, 1);
    }
    if ((dif_img = diff_image(img_p, img_q)))
        image_copy(img_q, img_p);
    return dif_e + dif_path + dif_img;
}


/**
 * Compare results of different shortest path algorithms
 */
PPMImage image_resize_compare(ShortestPath function1, ShortestPath function2,
        PPMImage *image, int width, int height)
{
    PPMImage temp_x1 = image_new_from(image);

    if (!resize_pre_conditions(image, width, height))
        return temp_x1;

    PPMImage temp_x2 = image_new_from(image);
    int *path1, *path2, step;
    Color color1, color2;

    int fails_x = 0, fails_y = 0;

    if (width)
    {
        RED(&color1, width);
        CYAN(&color2, width);
        path1 = calloc(temp_x1.height, sizeof(int));
        path2 = calloc(temp_x2.height, sizeof(int));
        step = width;
        while(step--)
        {
            function1(&temp_x1, path1);
            function2(&temp_x2, path2);

            if (differ('x', width - step, path1, &temp_x1, path2, &temp_x2)) {
                image_copy(&temp_x2, &temp_x1);
                image_draw_path(image, &temp_x2, path2, &color2);
                image_draw_path(image, &temp_x1, path1, &color1);
                image_remove_path(&temp_x1, path1);
                image_remove_path(&temp_x2, path1);
                fails_x++;
            } else {
                image_remove_path(&temp_x1, path1);
                image_remove_path(&temp_x2, path2);
            }
        };

        free(path1);
        free(path2);
    }

    if (height)
    {
        BLUE(&color1, height);
        YELLOW(&color2, height);
        PPMImage temp_y1 = image_new_transposed(&temp_x1);
        PPMImage temp_y2 = image_new_transposed(&temp_x1);

        PPMImage temp_t1 = image_new_transposed(image);
        PPMImage temp_t2 = image_new_transposed(&temp_t1);
        diff_image(&temp_t2, image);
        image_free(&temp_t2);

        path1 = calloc(temp_y1.height, sizeof(int));
        path2 = calloc(temp_y2.height, sizeof(int));
        step = height;
        while(step--)
        {
            function1(&temp_y1, path1);
            function2(&temp_y2, path2);
            if (differ('y', width - step, path1, &temp_y1, path2, &temp_y2)) {
                image_draw_tpath(&temp_t1, &temp_y2, path2, &color2);
                image_draw_tpath(&temp_t1, &temp_y1, path1, &color1);
                image_copy(&temp_y2, &temp_y1);
                image_remove_path(&temp_y1, path1);
                image_remove_path(&temp_y2, path1);
                fails_y++;
            } else {
                image_draw_tpath(&temp_t1, &temp_y1, path1, &color1);
                image_remove_path(&temp_y1, path1);
                image_remove_path(&temp_y2, path2);
            }
        };
        image_copy_transposed(&temp_x1, &temp_y1);
        image_copy_transposed(image, &temp_t1);
        image_free(&temp_t1);
        free(path1);
        free(path2);
        image_free(&temp_y1);
        image_free(&temp_y2);
    }
    image_free(&temp_x2);

    fprintf(stderr, "## Fails in X: %d/%d\n", fails_x, width);
    fprintf(stderr, "## Fails in Y: %d/%d\n", fails_y, height);

    return temp_x1;
}




