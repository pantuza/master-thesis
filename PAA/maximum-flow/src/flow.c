#include "flow.h"
#include "queue.h"
#include "champ.h"
#include "stdlib.h"
#include <limits.h>
#include <math.h>

/* min macro */
#define min(x,y) (((x)<(y)) ? (x) : (y))


/**
 * Global variables 
 */
int n;
int nvertexes = 0;
int relation[100][100];
double total_victories = 0;
int flow_teams = 0;


/**
 * Breadth-firt Search implementation
 * Finds the augment paths
 */
int bfs (int start, int target)
{
    /* Sets all vertices with white color */
    for (int u = 0; u < n; u++)
        color[u] = WHITE;

    /* Initialize the Queue */
    head = tail = 0;
    enqueue(start);
    pred[start] = -1;

    while (head!=tail) 
    {
        int u = dequeue();
        /**
         * Enqueue v if there exists a capacity from u to v in
         * the residual graph and it is positive
         */
        for (int v = 0; v < n; v++) 

            if (color[v]==WHITE && capacity[u][v]-flow[u][v]>0) 
            {
                enqueue(v);
                pred[v] = u;
            }
    }

    /* It reaches the sink if the color of sink is black */
    return color[target] == BLACK;
}


/**
 * Ford Fulkerson algorithm implementation
 */
int ford_fulkerson(int source, int sink)
{
    /* Initialize an empty flow */
    int max_flow = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            flow[i][j] = 0;

    /* While exists augment paths */
    while (bfs(source,sink)) 
    {
        /* calculate the amount that the flow can be incremented */
        int increment = INT_MAX;
        for (int u = n - 1; pred[u] >= 0; u = pred[u])
            increment = min(increment, capacity[pred[u]][u] - flow[pred[u]][u]);
    
        /* increments the flow */
        for (int u = n - 1; pred[u] >= 0; u = pred[u]) 
        {
            flow[pred[u]][u] += increment;
            flow[u][pred[u]] -= increment;
        }
        max_flow += increment;
    }

    return max_flow;
}


/**
 * Empty flows, capacities, colors and augment paths (pred)
 */
void empty()
{
    for (int i = 0; i < n; i++)
    {
        color[i] = WHITE;
        pred[i] = 0;
        for (int j = 0; j < n; j++)
        {
            flow[i][j] = 0;
            capacity[i][j] = 0;
        }
    }
}


/**
 * Allocates variables:
 *     color;
 *     aug_path;
 *     capacity;
 *     flow;
 *     Queue/
 */
void allocate(int size)
{
    n = size; 
    nvertexes = 0;
    int dimension =  n * sizeof(int);
    color = malloc(dimension);
    pred = malloc(dimension);

    capacity = malloc(n * sizeof(int *));
    flow = malloc(n * sizeof(int *));
    
    for(int i = 0; i < n; i++)
    {
        capacity[i] = malloc(dimension);
        flow[i] = malloc(dimension);
    }

    Queue = malloc((n + 2) * sizeof(int));
    empty();
}


/**
 * Adds a vertex to the graph
 */
int add_vertex()
{
    if(nvertexes < (n - 1))
        return ++nvertexes;
    else
    {
        fprintf(stderr, "Don't do that!");
        exit(EXIT_FAILURE);
    }
    return 0;
}


/** 
 * Adds an edge to the graph
 */
void add_edge(int u, int v, int weight)
{
    capacity[u][v] = weight;
}


/**
 * Counts the number of plays
 */
int count_plays(Champ *champ, int current)
{
    int num_plays = 0;
    for(int i=0; i < champ->nteams; i++)
        if(i != current)
            for(int j=i+1; j < champ->nteams; j++)
                if(j != current && champ->opponents[i][j] > 0)
                    num_plays++;
    return num_plays;
}


/**
 * Add plays and teams edges/vertexes
 * Verify if there exists a directed block for a team
 */
int build_graph(Champ *champ, int current, int source, 
                int sink, int *teams, int *blocked_by)
{
    // W is the current team total victories possible.
    int W = champ->teams[current].victories + champ->teams[current].remaining;

    // add plays and teams edges/vertexes 
    for(int i=0; i < champ->nteams; i++)
    
        if(i != current)
        {
            teams[i] = add_vertex(); 
            // W - wi
            int residual = W - champ->teams[i].victories;

            if(residual < 0)
            {
                *blocked_by = i;
                return 0;
            }
            add_edge(teams[i], sink, residual);
        }

    for(int i=0; i < champ->nteams; i++)
        if(i != current)
            for(int j=i+1; j < champ->nteams; j++)

                if(j != current && champ->opponents[i][j] > 0)
                {
                    int play = add_vertex();
                    add_edge(source, play, champ->opponents[i][j]);
                    add_edge(play, teams[i], INT_MAX);
                    add_edge(play, teams[j], INT_MAX);
                    relation[i][j] = play;
                }
    return 1;
}


/**
 * Calculates the cut of the set R on the graph
 */
int cut_R(Champ *champ, int current, int sink, int *teams, int *teams_in_R)
{
    total_victories = 0;
    flow_teams = 0;
    int R_i = 0; // index for insertion in teams_in_R

#ifdef MYDEBUG
    printf("R{ ");
#endif

    for(int i = 0; i < champ->nteams; i++)
        
        if(i != current && flow[teams[i]][sink] == capacity[teams[i]][sink])
        {

#ifdef MYDEBUG
            printf("%s ", champ->teams[i].name);
#endif
            
            /* Add team to the R set */
            teams_in_R[R_i++] = i;
            flow_teams++;
            total_victories += (double) (champ->teams[i].victories);
        }

#ifdef MYDEBUG
    printf("}\n");
#endif

    teams_in_R[R_i] = -1;
   
    /**
     * if there exists a game between two teams, 
     * calculates the total victories
     */
    for(int i = 0; i < R_i - 1; i++)
        for(int j = i + 1; j < R_i; j++)
            if(champ->opponents[i][j] > 0)
                /*
                 * R = Team nodes in the sink side
                 * T = wins(R) + remaining(R) 
                 */
                total_victories += (double) champ->opponents[i][j];

    /* cut(R) = T / |R| */
    return (int) ceil((double) total_victories / (double) flow_teams);
}


/**
 * Answer for the direct elimination of a team
 */
void direct_elimination(Champ *champ, int current, int blocked_by)
{
    printf("%s foi eliminado por %s\n", champ->teams[current].name,
            champ->teams[blocked_by].name);
    printf("Ele pode ganhar, no máximo, %d + %d = %d jogos\n",
            champ->teams[current].victories,
            champ->teams[current].remaining,
            champ->teams[current].victories + 
            champ->teams[current].remaining);
    printf("%s ganhou um total de %d jogos\n\n", 
            champ->teams[blocked_by].name,
            champ->teams[blocked_by].victories);
}


/**
 * Answer for the analitical elimination of a team
 */
void analitical_elimination(Champ *champ, int current, int *teams_in_R)
{
    printf("%s é eliminado\n", champ->teams[current].name);
    printf("Ele pode ganhar, no máximo, %d + %d = %d jogos\n", 
            champ->teams[current].victories,
            champ->teams[current].remaining,
            champ->teams[current].victories +
            champ->teams[current].remaining);

    int k = 0; // index in R
    int sum_vic = 0; // summation of victories in R
    printf("{ ");
    int team = k;
    while((team = teams_in_R[k++]) != -1)
    {
        printf("%s ", champ->teams[team].name);
        sum_vic += champ->teams[team].victories;
    }
    printf("} ");
    printf("ganharam um total de %d jogos\n", sum_vic);

    printf("Assim, em média, cada equipe vence %2.f/%d = %.2f jogos\n\n",
            total_victories, flow_teams,
            ((double) total_victories / flow_teams));
}


/**
 * Make the graph analisys based on the current team 
 */
void champ_analisys(Champ *champ, int current)
{
#ifdef MYDEBUG
    printf("Analising: %s\n", champ->teams[current].name);
#endif


    // Receives the count of plays
    int num_plays = count_plays(champ, current);

    // maps vertexes and teams
    int teams[champ->nteams];
    allocate((champ->nteams - 1) + num_plays + 2);

    // Creates source and sink nodes
    int source = 0;
    int sink = n - 1;

    int blocked_by = 0;
    // Build edges and vertexes
    if(!build_graph(champ, current, source, sink, teams, &blocked_by))
    {    
        direct_elimination(champ, current, blocked_by);
        return;
    }

    // Gets the Ford Fulkerson algorithm result
    ford_fulkerson(source, sink);

    // array with the teams inside the R set
    int teams_in_R[champ->nteams];
    /* Receives the cut value of the sink side */
    int cut = cut_R(champ, current, sink, teams, teams_in_R);
    
    /* Current Victories */
    int curr_victories = champ->teams[current].victories + 
                         champ->teams[current].remaining;


#ifdef MYDEBUG
    printf("cut: %d\tcurrent victories: %d\n", cut, curr_victories);
#endif


    /* Eliminates team */
    if(cut > curr_victories)
        analitical_elimination(champ, current, teams_in_R);


#ifdef MYDEBUG
    for(int i = 0; i < champ->nteams; i++)
        if(i != current)
            printf("%s(%d)=%d\n",champ->teams[i].name, teams[i], 
                    flow[teams[i]][n-1]);
    
    for(int i = 0; i < champ->nteams; i++)
        if(i != current)
        {
            printf("E->%d: %d - %d\n",i, flow[teams[i]][sink], 
                    capacity[teams[i]][sink]);
        }

    for(int i=0; i < champ->nteams; i++)
        if(i != current)
            for(int j=i+1; j < champ->nteams; j++)

                if(j != current && champ->opponents[i][j] > 0)
                {
                    printf("jogo: %dx%d --> %d = %d\n", i, j, i, 
                            flow[relation[i][j]][teams[i]]);
                    printf("jogo: %dx%d --> %d = %d\n", i, j, j,  
                            flow[relation[i][j]][teams[j]]);
                }

#endif
}


/**
 * Calculate the maximum flow for each team in the championship
 */
void maximum_flow(Champ *champ)
{
    for(int i = 0; i < champ->nteams; i++)
    {
        /* Ford fulkerson algorithm */
        champ_analisys(champ, i);
    }
}


