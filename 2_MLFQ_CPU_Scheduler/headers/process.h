#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int arrival_time;
    char type;
    int id;
    int burst_time;
    int start_time;
    int end_time;
    int remaining_time;
} Process;

Process *process_create(char type, int id, int arrival_time, int burst_time);
void process_destroy(Process *p);

#endif