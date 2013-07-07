#include "flow.h"
#include "queue.h"
#include "champ.h"
#include "stdlib.h"
#include <limits.h>
#include <math.h>
#define oo INT_MAX

int n;  // number of nodes

#define min(x,y) (((x)<(y)) ? (x) : (y))

int bfs (int start, int target) {
    int u,v;
    for (u=0; u<n; u++) {
    color[u] = WHITE;
    }   
    head = tail = 0;
    enqueue(start);
    pred[start] = -1;
    while (head!=tail) {
    u = dequeue();
        // Search all adjacent white nodes v. If the capacity
        // from u to v in the residual network is positive,
        // enqueue v.
    for (v=0; v<n; v++) {
        if (color[v]==WHITE && capacity[u][v]-flow[u][v]>0) {
        enqueue(v);
        pred[v] = u;
        }
    }
    }
    // If the color of the target node is black now,
    // it means that we reached it.
    return color[target]==BLACK;
}

int ford_fulkerson(int source, int sink)
{
    int i,j,u;
    // Initialize empty flow.
    int max_flow = 0;
    for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
        flow[i][j] = 0;
    }
    }
    // While there exists an augmenting path,
    // increment the flow along this path.
    while (bfs(source,sink)) {
        // Determine the amount by which we can increment the flow.
    int increment = oo;
    for (u=n-1; pred[u]>=0; u=pred[u]) {
        increment = min(increment,capacity[pred[u]][u]-flow[pred[u]][u]);
    }
        // Now increment the flow.
    for (u=n-1; pred[u]>=0; u=pred[u]) {
        flow[pred[u]][u] += increment;
        flow[u][pred[u]] -= increment;
    }
    max_flow += increment;
    }
    // No augmenting path anymore. We are done.
    return max_flow;
}

int nvertexes = 0;

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
 */
void build_graph(Champ *champ, int current, int source, int sink, int *teams)
{
    // W is the current team total victories possible.
    int W = champ->teams[current].victories + champ->teams[current].remaining;

    // add plays and teams edges/vertexes 
    for(int i=0; i < champ->nteams; i++)
    
        if(i != current)
        {
            teams[i] = add_vertex(); // W - wi
            add_edge(teams[i], sink, W - champ->teams[i].victories);
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
                }
}

int cut_R(Champ *champ, int current, int sink, int *teams)
{
    double total_victories = 0;
    int flow_teams = 0;

    printf("R{ ");
    for(int i = 0; i < champ->nteams; i++)
        
        if(i != current && flow[teams[i]][sink] != 0)
        {
            printf("%s ", champ->teams[i].name);
            flow_teams++;
            /*
             * R = Team nodes in the sink side
             * T = wins(R) + remaining(R) 
             */
            total_victories += (double) (champ->teams[i].victories + 
                               champ->teams[i].remaining / 2.0);
        }
    printf("}\n");
    
    /* cut(R) = T / |R| */
    return (int) ceil((double) total_victories / (double) flow_teams);
}

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

    // Build edges and vertexes
    build_graph(champ, current, source, sink, teams);

    // Gets the Ford Fulkerson algorithm result
    int max_flow = ford_fulkerson(source, sink);

#ifdef MYDEBUG
    printf("max_flow: %d\n", max_flow);
#endif

    /* Receives the cut value of the sink side */
    int cut = cut_R(champ, current, sink, teams);
    
    /* Current Victories */
    int curr_victories = champ->teams[current].victories + 
                         champ->teams[current].remaining;

#ifdef MYDEBUG
    printf("cut: %d\tcurrent victories: %d\n", cut, curr_victories);
#endif

    /* Eliminates team */
    if(cut > curr_victories)
    {
        printf("Eliminated Team: %s\n", champ->teams[current].name);
    }

#ifdef MYDEBUG
    for(int i = 0; i < champ->nteams; i++)
        if(i != current)
            printf("%s(%d)=%d\n",champ->teams[i].name, teams[i], flow[teams[i]][n-1]);
#endif
}

void maximum_flow(Champ *champ)
{
    for(int i = 0; i < champ->nteams; i++)
    {
        /* Ford fulkerson algorithm */
        champ_analisys(champ, i);
    }
}


