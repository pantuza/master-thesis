#include "flow.h"
#include "queue.h"
#include "champ.h"
#include "stdlib.h"
#include <limits.h>

int bfs (int start, int target, int n)
{
    int u,v;
    for (u=0; u<n; u++)
        color[u] = WHITE;
    
    head = tail = 0;
    enqueue(start);
    aug_path[start] = -1;
    
    while (head != tail) 
    {
        u = dequeue();
        // Search all adjacent white nodes v. If the capacity
        // from u to v in the residual network is positive,
        // enqueue v.
        for (v = 0; v < n; v++) 
            if (color[v] == WHITE && capacity[u][v] - flow[u][v] > 0) 
            {
                enqueue(v);
                aug_path[v] = u;
            }
    }
    // If the color of the target node is black now,
    // it means that we reached it.
    return (color[target] == BLACK);
}

int ford_fulkerson(int source, int sink, int n)
{
    int i,j,u;
    // Initialize empty flow.
    int max_flow = 0;
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
            flow[i][j] = 0;

    // While there exists an augmenting path,
    // increment the flow along this path.
    while (bfs(source,sink, 0)) 
    {
        // DeterMINe the amount by which we can increment the flow.
        int increment = INT_MAX;
        for (u=n-1; aug_path[u]>=0; u=aug_path[u])
            increment = MIN(increment,capacity[aug_path[u]][u]-flow[aug_path[u]][u]);
        // Now increment the flow.
        for (u=n-1; aug_path[u]>=0; u=aug_path[u]) 
        {
            flow[aug_path[u]][u] += increment;
            flow[u][aug_path[u]] -= increment;
        }
        
        max_flow += increment;
    }
    // No augmenting path anymore. We are done.
    return max_flow;
}


void maximum_flow(Champ *champ)
{

    capacity = malloc(champ->nteams * sizeof(int *) 
                      + champ->nteams * sizeof(int));
    flow = malloc(champ->nteams * sizeof(int *) 
                  + champ->nteams * sizeof(int));
        
    color = malloc(champ->nteams * sizeof(int));
    aug_path = malloc(champ->nteams * sizeof(int));
        
    for(int i = 0; i < champ->nteams; i++)
    {

        /* Ford fulkerson algorithm */
        ford_fulkerson(1, 2, i);
    }
}
