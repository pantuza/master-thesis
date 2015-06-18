/**
 * Simple Queue implementation used on the 
 * Breadth-First Search to control augment paths.
 *
 * @author: Gustavo Pantuza
 * @since: 21.06.2013
 */


#include "queue.h"
#include "flow.h"


/**
 * Add element x to the tail of the queue and 
 * updates the tail value
 */
void enqueue(int x)
{
    Queue[tail] = x;
    tail++;
    color[x] = GRAY;
}


/**
 * Returns the element on the head of the queue 
 * and updates the head value
 */
int dequeue()
{
    int x = Queue[head];
    head++;
    color[x] = BLACK;
    return x;
}
