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

int resize_pre_conditions(const PPMImage *, const int, const int);
PPMImage image_resize(ShortestPath, const PPMImage *, int , int);
PPMImage image_resize_preview(ShortestPath, PPMImage *, int, int);

#endif /* RESIZE_H_ */
