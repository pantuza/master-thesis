/*
 * test.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "graph.h"
#include "dynamic.h"
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
                fprintf(stderr, "#%d[%d,%d]:p1.color(%d,%d, %d)"
                        "!= p2.color(%d,%d, %d)\n",
                        ++difc, x,y,p1.R,p1.G,p1.B,p2.R,p2.G,p2.B);
            if (p1.energy != p2.energy)
                fprintf(stderr, "$%d[%d,%d]:p1.e(%.2f) != p2.e(%.2f)\n",
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
        fprintf(stderr, "[%c:%d] Energy 1:%.2f > 2:%.2f!\n", d, i, ep, eq);
    }else
        if (ep  < eq) {
            dif_e = 1;
            fprintf(stderr, "[%c:%d] Energy 1:%.2f < 2:%.2f!\n", d, i, ep, eq);
        } else
            if (printEquals)
                fprintf(stderr, "[%c:%d] Energy equals %.2f!\n", d, i, ep);
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
PPMImage test_correction(PPMImage *image, int width, int height)
{
    PPMImage temp_x1 = image_new_from(image);

    if (!resize_pre_conditions(image, width, height))
        return temp_x1;

    ShortestPath *function1, *function2;
    function1 = &(dp_shortest_path);
    function2 = &(graph_shortest_path);

    PPMImage temp_x2 = image_new_from(image);
    int *path1, *path2, step;
    Color color1, color2;

    int fails_x = 0, fails_y = 0;

    if (width)
    {
        RED(&color1, width);
        GREEN(&color2, width);
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

#define LOG2(n) (log((double)n)/log(2.0))
typedef double TO(int, int);

double O_gr(int d1, int d2)
{
    double O, O_wh, O_whl, O_w, O_h;
    O_wh = (double)d1*(double)d2;
    O_whl = O_wh * (double)LOG2(d1*d2);
    O_w = (double)d1;
    O_h = (double)d2;
    O = O_wh + O_w * O_whl + O_h;
    return O;
}

double O_dp(int d1, int d2)
{
    double O, O_wh, O_w, O_h;
    O_wh = (double)d1*d2;
    O_w = (double)d1;
    O_h = (double)d2;
    O = O_wh + O_w + O_h;
    return O;
}

double complexity(TO O, int w, int h, int dw, int dh)
{
    double c = 0;
    if (dw)
        for (int i = w; i >= w - dw; i--) {
            c += O(i,h);
        }
    w = w - dw;
    if (dh)
        for (int i = h; i >= h - dh; i--) {
            c += O(w,i);
       }
    return c;
}

/**
 * Compare results of different shortest path algorithms
 */
PPMImage test_times(PPMImage *image, int width, int height,
        TO O, ShortestPath function,
        double *d, double *c, double *t, double *r)
{
    clock_t start, end;

    *c = complexity(O, image->width, image->height, width, height);
    start = clock();
    PPMImage temp = image_resize(function, image, width, height);
    end = clock();
    *d = MAX(end - start, 0.01);
    *t = (double)*d / (double)CLOCKS_PER_SEC;
    *r = (double)*c / (double)*d;
    return temp;
}

void print_resize1(FILE *out, char *filename,
        PPMImage *image, int width, int height)
{
    fprintf(out, "[%s] I(%d,%d) - delta(%d,%d)\n",
            filename, image->width, image->height, width, height);
}

void print_times1(FILE *out, const char *title,
        double d, double c, double t, double r)
{
    fprintf(out, "[%s] complexity: %.2f\n", title, c);
    fprintf(out, "[%s] time: %.2f s (%.2f/%ld) \n",
            title, t, d, CLOCKS_PER_SEC);
    fprintf(out, "[%s] complexity / time: %.2f\n", title, r);

}


void print_result1(FILE *out, double rc, double rt, double rr)
{
    fprintf(out, "[!!] complexity_gr / complexity_df: %.2f\n", rc);
    fprintf(out, "[!!] time_gr / time_df: %.2f\n", rt);
    fprintf(out, "[!!] r_gr / r_df: %.2f\n", rr);
}

void print_resize(FILE *out, char *filename,
        PPMImage *image, int width, int height)
{
    fprintf(out, "%s, %d, %d, %d, %d",
            filename, image->width, image->height, width, height);
}

void print_times(FILE *out, const char *title,
        double d, double c, double t, double r)
{
    //fprintf(stderr, ", %s, %.2f, %.2f, %.2f", title, c, d, r);
    fprintf(out, ", %.2f, %.2f, %.2f", c, d, r);
}

void print_result(FILE *out, double rc, double rt, double rr)
{
    fprintf(out, ", %.2f, %.2f, %.2f\n", rc, rt, rr);
}

/**
 * Compare results of different shortest path algorithms
 */
PPMImage test_time(char *output, char *filename,
        PPMImage *image, int width, int height)
{
    double t_1, t_2;
    double r_1, r_2;
    double d_1, d_2;
    double c_1, c_2;

    PPMImage temp1 = test_times(image, width, height,
            O_dp, dp_shortest_path,
            &d_1, &c_1, &t_1, &r_1);
    PPMImage temp2 = test_times(image, width, height,
            O_gr, graph_shortest_path,
            &d_2, &c_2, &t_2, &r_2);
    image_copy(image, &temp2);
    image_free(&temp2);

    FILE *out = fopen(output, "a");
    print_resize(out, filename, image, width, height);
    print_times(out, "dp", d_1, c_1, t_1, r_1);
    print_times(out, "gr", d_2, c_2, t_2, r_2);
    print_result(out, (double)c_2 / (double)c_1, t_2/t_1, r_2/r_1);
    fclose(out);

    return temp1;
}


