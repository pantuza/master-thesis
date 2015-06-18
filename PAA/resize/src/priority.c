/*
 * priority.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 *  Based on: http://rosettacode.org/wiki/Priority_queue#C
 */
#include <stdio.h>
#include <stdlib.h>

#include "priority.h"

/* first element in array not used to simplify indices */
void priq_init(pri_queue q, int size)
{
    if (size < 4) size = 4;

    q->buf = malloc(sizeof(q_elem_t) * size);
    q->n = 1;

}

void priq_push(pri_queue q, data_t data, priq_key_t pri)
{
    q_elem_t *b;
    int n, m;

    b = q->buf;

    n = q->n++;
    /* append at end, then up heap */
    while ((m = n / 2) && pri < b[m].pri) {
        b[n] = b[m];
        n = m;
    }
    b[n].data = data;
    b[n].pri = pri;
}

/* remove top item. returns 0 if empty. *pri can be null. */
data_t priq_pop(pri_queue q)
{
    data_t out;
    if (q->n == 1) return 0;

    q_elem_t *b = q->buf;

    out = b[1].data;

    /* pull last item to top, then down heap. */
    --q->n;

    int n = 1, m;
    while ((m = n * 2) < q->n) {
        if (m + 1 < q->n && b[m].pri > b[m + 1].pri) m++;

        if (b[q->n].pri <= b[m].pri) break;
        b[n] = b[m];
        n = m;
    }

    b[n] = b[q->n];

    return out;
}

