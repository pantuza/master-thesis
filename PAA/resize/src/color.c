/*
 * color.c
 *
 *  Created on: 13/06/2013
 *      Author: Gustavo Pantuza
 *
 *  Ref: http://en.wikipedia.org/wiki/HSV_color_space
 */

#include <math.h>
#include <stdlib.h>
#include "color.h"

/*
 * Ref:  http://www.cs.rit.edu/~ncs/color/t_convert.html
 * r,g,b values are from 0 to 1
 * h = [0,360], s = [0,1], v = [0,1]
 * if s == 0, then h = -1 (undefined)
 */
void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
    int i;
    float f, p, q, t;
    if( s == 0 ) {
        // achromatic (grey)
        *r = *g = *b = v;
        return;
    }
    h /= 60;            // sector 0 to 5
    i = floor( h );
    f = h - i;          // factorial part of h
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );
    switch( i ) {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:        // case 5:
            *r = v;
            *g = p;
            *b = q;
            break;
    }
}

void Hue2RGB(Color *color)
{
    float r, g, b;
    HSVtoRGB(&r, &g, &b, color->hue, 1, 1);
    color->r = 255 * r;
    color->g = 255 * g;
    color->b = 255 * b;
}

void init_color(Color *color, const int num_colors)
{
    color->step = (360/abs(num_colors));
    if (color->step < 1)
        color->step = 1;
    color->hue = 0;
    Hue2RGB(color);
}

void next_color(Color *color)
{
    color->hue += color->step;
    if (color->hue > 360)
        color->hue = 360 - color->hue;
    Hue2RGB(color);
}

