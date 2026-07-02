#include <string.h>
#include "headers/input.h"

int input_next_event(FILE *fp, Event *event) {
    char type_str[32];
    int count;

    count = fscanf(fp, "%d %31s", &event->arrival_time, type_str);

    if (count == EOF || count == -1) {
        event->type = EVENT_EOF;
        return 0;
    }

    if (count != 2) {
        return -1;
    }

    if (strcmp(type_str, "PRINTALL") == 0) {
        event->type = EVENT_PRINTALL;
        event->process_type = '\0';
        event->id = 0;
        event->burst_time = 0;
        return 1;
    }

    event->type = EVENT_PROCESS;
    event->process_type = type_str[0];

    count = fscanf(fp, "%d %d", &event->id, &event->burst_time);

    if (count != 2) {
        return -1;
    }

    return 1;
}