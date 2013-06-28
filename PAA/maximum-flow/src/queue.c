#include "queue.h"
#include "flow.h"


void enqueue(int x)
{
    Queue[tail] = x;
    tail++;
    color[x] = GRAY;
}

int dequeue()
{
    int x = Queue[head];
    head++;
    color[x] = BLACK;
    return x;
}
