#include <stdio.h>
#include <stdlib.h>
#include "champ.h"
#include "debug.h"



/**
 * Loads the data from file to a Champ structure 
 */
void load(FILE *file, Champ *champ)
{
    /* Reads the number of teams in the championship */
    fscanf(file, "%d\n", &champ->nteams);
   
    /* Allocates the teams array */
    champ->teams = malloc(champ->nteams * sizeof(Team *));

    /* Allocates the opponents matrix */
    champ->opponents = malloc(champ->nteams * sizeof(int));
    for(int i = 0; i < champ->nteams; i++)
        champ->opponents[i] = malloc(champ->nteams * sizeof(int));

    int current = 0;
    int result = champ->nteams;
    do
    {
        /* Allocates a Team type on memory */
        Team *team = malloc(sizeof(Team));
        
        /* Fill the team values */
        fscanf(file, 
               "%s %d %d %d", 
               team->name,
               &team->victories,
               &team->loss,
               &team->remaining);

        champ->teams[current] = team;

        DEBUG(printf("name: %s\tvictories: %d\tloss: %d\tremaining: %d\t",
                     team->name, team->victories, 
                     team->loss, team->remaining));

        /* Fill the opponents values for the current team */
        for(int i = 0; i < champ->nteams; i++)
            fscanf(file, "%d", &champ->opponents[current][i]);
        
        DEBUG(
            printf("opponents: [%d", champ->opponents[current][0]);
            for(int i = 1; i < champ->nteams - 1; i++)
            {
                printf(", %d", champ->opponents[current][i]);
            }
            printf(", %d]\n", champ->opponents[current][champ->nteams - 1]);
        );

        fscanf(file, "\n");
        current++;

    } while(--result);
}

void save(FILE *file, Champ *champ)
{

}
