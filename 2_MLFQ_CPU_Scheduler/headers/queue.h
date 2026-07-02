#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

typedef struct queue_node {
    Process *process;
    struct queue_node *next;
} QueueNode;

typedef struct {
    QueueNode *head;
    QueueNode *tail;
} Queue;


void queue_init(Queue *q);
int queue_is_empty(const Queue *q);
void queue_push(Queue *q, Process *p);
Process *queue_pop(Queue *q);
void queue_clear(Queue *q);

#endif