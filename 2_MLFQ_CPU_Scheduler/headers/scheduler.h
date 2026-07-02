#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"
#include "cpu.h"


typedef struct {
    Queue ready[3];
    Queue finished;

    Cpu cpu;

    int time_slice_used;
    int current_time;
} Scheduler;


void scheduler_init(Scheduler *s);
void scheduler_destroy(Scheduler *s);
void scheduler_add_process(Scheduler *s, Process *p);
void scheduler_tick(Scheduler *s);
void scheduler_print(const Scheduler *s);
int scheduler_is_done(const Scheduler *s);

#endif