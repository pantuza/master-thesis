#include <stdio.h>
#include <stdlib.h>
#include "champ.h"
#include "debug.h"



/**
 * Loads the data from file to a Graph structure 
 */
void load(FILE *file, Graph *graph)
{
    /* Reads the number of teams in the championship */
    fscanf(file, "%d\n", &graph->nteams);
   
    /* Allocates the teams array */
    graph->teams = malloc(graph->nteams * sizeof(Team *));

    /* Allocates the opponents matrix */
    graph->opponents = malloc(graph->nteams * sizeof(int));
    for(int i = 0; i < graph->nteams; i++)
        graph->opponents[i] = malloc(graph->nteams * sizeof(int));

    int current = 0;
    int result = graph->nteams;
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

        graph->teams[current] = team;

        DEBUG(printf("name: %s\tvictories: %d\tloss: %d\tremaining: %d\t",
                     team->name, team->victories, 
                     team->loss, team->remaining));

        /* Fill the opponents values for the current team */
        for(int i = 0; i < graph->nteams; i++)
            fscanf(file, "%d", &graph->opponents[current][i]);
        
        DEBUG(
            printf("opponents: [%d", graph->opponents[current][0]);
            for(int i = 1; i < graph->nteams - 1; i++)
            {
                printf(", %d", graph->opponents[current][i]);
            }
            printf(", %d]\n", graph->opponents[current][graph->nteams - 1]);
        );

        fscanf(file, "\n");
        current++;

    } while(--result);
}

void save(FILE *file, Graph *graph)
{

}
