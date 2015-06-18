#ifndef QUEUE_H
#define QUEUE_H

int head;
int tail;
int *Queue;
int *color;


/* Public Functions */
void enqueue(int);
int dequeue();
#endif
