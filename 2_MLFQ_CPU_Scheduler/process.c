#include <stdio.h>
#include <stdlib.h>
#include "headers/process.h"

/* Creates and initializes a new Process using dynamic memory allocation. */
Process *process_create(char type, int id, int arrival_time, int burst_time) {

    Process * proc = malloc(sizeof(Process));

    if (proc == NULL)  {return NULL;}

    proc->type = type;
    proc->id = id;
    proc->arrival_time = arrival_time;
    proc->burst_time   = burst_time;
    proc->remaining_time = burst_time;
    proc->start_time     = -1;
    proc->end_time       = -1;

    return proc;
}


void process_destroy(Process *p) {
    free(p);
}