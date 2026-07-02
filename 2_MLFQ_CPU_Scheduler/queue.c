#include <stdio.h>
#include <stdlib.h>
#include "headers/queue.h"


void queue_init(Queue *q) {

    q->head = NULL;
    q->tail = NULL;
}


int queue_is_empty(const Queue *q) {

    return (q->head == NULL);
}


void queue_push(Queue *q, Process *p) {

    QueueNode * new_node = malloc ( sizeof(QueueNode) );

    new_node->process = p;
    new_node->next    = NULL;

    if (queue_is_empty(q)) 
    {
        q->head = new_node;
        q->tail = new_node;
    } 
    else 
    {
        q->tail->next = new_node;
        q->tail       = new_node;
    }
}

Process *queue_pop(Queue *q) {

    QueueNode *start   = q->head;
    Process   *ptr = start->process;

    q->head = start->next;

    if (q->head == NULL) {
        q->tail = NULL;
    }

    free(start);
    return ptr;
}

void queue_clear(Queue *q) {

    while (!queue_is_empty(q)) {
        Process *ptr = queue_pop(q);
        process_destroy(ptr);
    }
}