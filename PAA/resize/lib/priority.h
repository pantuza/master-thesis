#ifndef PRIQ_H
#define PRIQ_H
/*
 * priority.h
 *
 *  Created on: 12/06/2013
 *      Author: dev
 */

#include "ppm.h"

typedef int data_t;
typedef Energy key_t;

typedef struct {
        data_t data;
        key_t pri;
} q_elem_t;

typedef struct {
        q_elem_t *buf;
        int n;
} pri_queue_t, *pri_queue;

void priq_init(pri_queue, int);
void priq_push(pri_queue, data_t, key_t);
data_t priq_pop(pri_queue);


#define priq_purge(q) ((q)->n = 1)
#define priq_size(q)  ((q)->n - 1)
#define priq_top(q)   ((q)->buf[1].data)
#define priq_free(q)  (free((q)->buf))

#endif
