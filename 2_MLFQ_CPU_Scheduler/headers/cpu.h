#ifndef CPU_H
#define CPU_H

#include "process.h"

typedef struct {
    Process *current;
} Cpu;


void cpu_init(Cpu *cpu);
void cpu_set_process(Cpu *cpu, Process *process);
Process *cpu_get_process(const Cpu *cpu);
void cpu_run_one_tick(Cpu *cpu, int current_time);

#endif