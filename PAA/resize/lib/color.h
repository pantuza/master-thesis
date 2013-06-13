/*
 * color.h
 *
 *  Created on: 13/06/2013
 *      Author: Gustavo Pantuza
 */

#ifndef COLOR_H_
#define COLOR_H_

typedef struct {
        int r, g, b;;
        float hue, step;
} Color;


void Hue2RGB(Color *);
void init_color(Color *, const int);
void next_color(Color *);


#endif /* COLOR_H_ */
