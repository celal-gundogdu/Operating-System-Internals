#include <stdio.h>
#include <stdlib.h>
#include "headers/input.h"
#include "process.h"
#include "headers/scheduler.h"


int main(int argc, char *argv[]) {
    FILE *fp;
    Event event;
    Scheduler scheduler;
    int status;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    scheduler_init(&scheduler);

    status = input_next_event(fp, &event);

    while (status == 1) {
        while (scheduler.current_time < event.arrival_time) {
            scheduler_tick(&scheduler);
        }

        if (event.type == EVENT_PROCESS) {
            Process *p = process_create(
                event.process_type,
                event.id,
                event.arrival_time,
                event.burst_time
            );
            scheduler_add_process(&scheduler, p);
        }
        else if (event.type == EVENT_PRINTALL) {
            scheduler_print(&scheduler);
        }

        status = input_next_event(fp, &event);
    }

    if (status == -1) {
        fprintf(stderr, "Invalid input format\n");
        fclose(fp);
        scheduler_destroy(&scheduler);
        return EXIT_FAILURE;
    }

    while (!scheduler_is_done(&scheduler)) {
        scheduler_tick(&scheduler);
    }

    fclose(fp);
    scheduler_destroy(&scheduler);

    return EXIT_SUCCESS;
}