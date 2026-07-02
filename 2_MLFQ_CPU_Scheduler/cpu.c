#include <stdlib.h>
#include "headers/cpu.h"


void cpu_init(Cpu *cpu) {
    cpu->current = NULL;
}


void cpu_set_process(Cpu *cpu, Process *process) {

    cpu->current = process;
}

/* Returns a pointer to the currently running process. */
Process *cpu_get_process(const Cpu *cpu) {
    
    Process * ptr = cpu->current;
    return ptr;
}

/* Executes the current process for one time unit. Records start_time on first execution tick and end_time when the process finishes (remaining_time == 0). */
void cpu_run_one_tick(Cpu *cpu, int current_time) {

    Process *proc = cpu->current;

    /* Record the start time on the first tick of execution */
    if (proc->remaining_time == proc->burst_time) {
        proc->start_time = current_time;
    }

    proc->remaining_time--;

    /* Record the end time when process completes */
    if (proc->remaining_time == 0) {
        proc->end_time = current_time + 1;
    }
}