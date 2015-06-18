/*
 * color.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 *  Ref: http://en.wikipedia.org/wiki/HSV_color_space
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "color.h"
#include "debug.h"

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

#define ABS_ANGLE(x) fmod(fmod(x, 360.0) + 360.0, 360.0);

static inline float __arch__(float a, float b, int growing)
{
    if (a == b)
        return 360.0;
    float min = (a < b? a: b);
    float max = (a > b? a: b);
    float aa = max - min;
    if ((growing && a > b) || (!growing && b > a))
        aa = 360.0 - aa;
    return aa;
}

void color_init_range(Color *color, const int num_colors,
        const float start, const float end, const int growing)
{
    color->start = ABS_ANGLE(start);
    color->end = ABS_ANGLE(end);
    float aa = __arch__(color->start, color->end, growing);
    color->step = fabs(aa/fabs((float)num_colors));
    if (!growing) // decreasing
        color->step = -color->step;
    color->hue = start;
    Hue2RGB(color);
}

void color_init(Color *color, const int num_colors)
{
    color_init_range(color, num_colors, 0.0, 360.0, 1);
}

void color_next(Color *color)
{
    color->hue = ABS_ANGLE(color->hue + color->step);
    if (color->step && color->end != color->start)
    {
        if (color->step > 0)
        {
            if ((color->end > color->start && color->hue > color->end) ||
                (color->end < color->start &&
                   (color->hue < color->start && color->hue > color->end)))
            {
                color->hue = color->start;
            }
        } else
        {
            if ((color->end < color->start && color->hue < color->end) ||
                (color->end > color->start &&
                   (color->hue < color->start && color->hue > color->end)))
            {
                color->hue = color->start;
            }
        }
    }
    Hue2RGB(color);
}

