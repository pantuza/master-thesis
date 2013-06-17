/*
 * resize.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */

#ifndef RESIZE_H_
#define RESIZE_H_

#include "ppm.h"

typedef void ShortestPath(PPMImage *, int *);

PPMImage image_resize(ShortestPath, const PPMImage *, int , int);
PPMImage image_resize_preview(ShortestPath, PPMImage *, int, int);

PPMImage image_resize_compare(ShortestPath function1, ShortestPath function2,
        PPMImage *image, int width, int height);

#endif /* RESIZE_H_ */
