#include <stdio.h>
#include <stdlib.h>
#include "headers/scheduler.h"


/* Returns the time quantum for a given process type. Q = 2^i where i is the queue level: A=2  B=4  C=8. */
static int get_quantum(char type) {

    if (type == 'A') return 2;
    else if (type == 'B') return 4;
    else return 8;
}

/* Returns the ready queue index based on process type. A -> 0 (highest), B -> 1 ,  C -> 2 (lowest). */
static int get_queue_index(const Process *p) {

    char type = p->type;

    if (type == 'A') return 0;
    else if (type == 'B') return 1;
    else return 2;
}

/* Finds the highest-priority non-empty ready queue. Returns 0, 1, or 2 on success: -1 if all empty. */
static int find_next_queue(const Scheduler *s) {

    for (int i = 0; i < 3; i++) {
        if (!queue_is_empty(&s->ready[i])) return i;
    }

    return -1;
}

/* Pop front process from the highest-priority non-empty queue and assigns it to the CPU. Resets the time slice counter. */
static void dispatch_next_process(Scheduler *s) {

    int idx = find_next_queue(s);
    if (idx == -1) {return;}

    Process *ptr = queue_pop(&s->ready[idx]);
    cpu_set_process(&s->cpu, ptr);

    s->time_slice_used = 0;
}

/* Initializes scheduler: all three ready queues, finished queue, CPU , time slice counter, and simulation clock. */
void scheduler_init(Scheduler *s) {


    for (int i = 0; i < 3; i++) {
        queue_init(&s->ready[i]);
    }

    queue_init(&s->finished);
    cpu_init(&s->cpu);

    s->time_slice_used = 0;
    s->current_time = 0;
}

/* Frees all processes in ready queues, finished queue and CPU. */
void scheduler_destroy(Scheduler *s) {

    int i;
    Process *running;

    for (i = 0; i < 3; i++) {
        queue_clear(&s->ready[i]);
    }

    queue_clear(&s->finished);

    running = cpu_get_process(&s->cpu);
    if (running != NULL) {
        process_destroy(running);
        cpu_set_process(&s->cpu, NULL);
    }
}

/* Adds a new process to the scheduler. If the new process has higher priority than the currently running one, preempt immediately. */
void scheduler_add_process(Scheduler *s, Process *p) {

    int new_idx;
    Process *running;

    new_idx = get_queue_index(p);
    running = cpu_get_process(&s->cpu);

    if (running != NULL) {
        int run_idx = get_queue_index(running);

        if (new_idx < run_idx)
        {
            /* Higher priority arrived: preempt running process */
            queue_push(&s->ready[run_idx], running);
            cpu_set_process(&s->cpu, NULL);
            queue_push(&s->ready[new_idx], p);
            s->time_slice_used = 0;
            dispatch_next_process(s);
        } 
        else 
        {
            /* Same or lower priority: just enqueue */
            queue_push(&s->ready[new_idx], p);
        }
    } 
    else 
    {
        /* CPU idle: enqueue then dispatch */
        queue_push(&s->ready[new_idx], p);
        dispatch_next_process(s);
    }
}

/* Advances simulation by one time unit. Handles quantum expiration (at the beginning), execution, and completion (at the end). */
void scheduler_tick(Scheduler *s) {
    Process *running = cpu_get_process(&s->cpu);

    /* Quantum check at the beginning of the new time instant */
    if (running != NULL &&
        s->time_slice_used >= get_quantum(running->type)) {
        queue_push(&s->ready[get_queue_index(running)], running);
        cpu_set_process(&s->cpu, NULL);
        s->time_slice_used = 0;
        running = NULL;
    }

    /* Dispatch next process if CPU is idle */
    if (running == NULL) {
        dispatch_next_process(s);
        running = cpu_get_process(&s->cpu);
    }

    /* Execute one tick */
    if (running != NULL) {
        cpu_run_one_tick(&s->cpu, s->current_time);
        s->time_slice_used++;

        /* If process just completed, move to finished immediately */
        if (running->remaining_time == 0) {
            queue_push(&s->finished, running);
            cpu_set_process(&s->cpu, NULL);
            s->time_slice_used = 0;
            dispatch_next_process(s);
        }
    }

    s->current_time++;
}

/* Prints the current snapshot: running, waiting, and finished processes. */
void scheduler_print(const Scheduler *s) {
    Process *running;
    QueueNode *node;
    int i;

    /* Running process */
    running = cpu_get_process(&s->cpu);
    printf("RP:");
    if (running != NULL) {
        printf(" %c%d", running->type, running->id);
    }
    printf("\n");

    /* Waiting processes across all three ready queues */
    printf("WP:");
    for (i = 0; i < 3; i++) {
        node = s->ready[i].head;
        while (node != NULL) {
            printf(" %c%d %d %d",
                   node->process->type,
                   node->process->id,
                   node->process->start_time,
                   node->process->end_time);
            node = node->next;
        }
    }
    printf("\n");

    /* Finished processes */
    printf("FP:");
    node = s->finished.head;
    while (node != NULL) {
        printf(" %c%d %d %d",
               node->process->type,
               node->process->id,
               node->process->start_time,
               node->process->end_time);
        node = node->next;
    }
    printf("\n");
}

/* Returns 1 if CPU is idle and all ready queues are empty. */
int scheduler_is_done(const Scheduler *s) {
    if (cpu_get_process(&s->cpu) != NULL) {
        return 0;
    }
    if (find_next_queue(s) != -1) {
        return 0;
    }
    return 1;
}