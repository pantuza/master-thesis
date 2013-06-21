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

PPMImage test_correction(PPMImage *, int, int);
PPMImage test_time(char *, char *, PPMImage *, int, int);

#endif /* TEST_H_ */
