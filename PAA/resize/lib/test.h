/*
 * test.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */

#ifndef TEST_H_
#define TEST_H_

#include "ppm.h"
#include "resize.h"

PPMImage image_resize_compare(ShortestPath function1, ShortestPath function2,
        PPMImage *image, int width, int height);

#endif /* TEST_H_ */
