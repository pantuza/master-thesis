#ifndef _CHAMP_H
#define _CHAMP_H

/* Edge type */
typedef struct Edge
{
    int source;
    int dive;
    int capacity;
} Edge;

/* Public functions */
void load(FILE *);
void save(FILE *);
#endif
