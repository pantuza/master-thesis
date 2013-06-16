/*
 * color.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#ifndef COLOR_H_
#define COLOR_H_

typedef struct {
        int r, g, b;;
        float hue, step;
} Color;


void Hue2RGB(Color *);
void color_init(Color *, const int);
void color_next(Color *);


#endif
