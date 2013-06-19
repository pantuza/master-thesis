#ifndef _CHAMP_H
#define _CHAMP_H

#define TEAM_NAME_SIZE 50


/* Edge type */
typedef struct Edge
{
    int source;
    int dive;
    int capacity;
} Edge;


/* Team type */
typedef struct Team
{
    char name[TEAM_NAME_SIZE];
    int victories;
    int loss;
    int remaining;
} Team;


/* Graph type */
typedef struct Graph
{
    int nteams;
    Team **teams;
    int **opponents;
} Graph;


/* Public functions */
void load(FILE *, Graph *);
void save(FILE *, Graph *);
#endif
