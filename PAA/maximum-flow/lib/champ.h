#ifndef _CHAMP_H
#define _CHAMP_H

#include <stdio.h>

#define TEAM_NAME_SIZE 50

/* Team type */
typedef struct Team
{
    char name[TEAM_NAME_SIZE];
    int victories;
    int loss;
    int remaining;
} Team;


/* Champ type */
typedef struct Champ
{
    int nteams;
    Team *teams;
    int **opponents;
} Champ;


/* Public functions */
void load(FILE *, Champ *);
void save(FILE *, Champ *);
#endif
