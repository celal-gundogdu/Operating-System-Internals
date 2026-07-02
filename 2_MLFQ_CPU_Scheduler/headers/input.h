#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

typedef enum {
    EVENT_PROCESS,
    EVENT_PRINTALL,
    EVENT_EOF
} EventType;


typedef struct {
    EventType type;
    int arrival_time;
    char process_type;
    int id;
    int burst_time;
} Event;


int input_next_event(FILE *fp, Event *event);

#endif