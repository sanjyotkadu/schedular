#include "schedular.h"
#include <stdio.h>



schedular_t schedular;

void schedular_init(get_ticks_t get_ticks) {
    uint16_t i;
    for (i = 0; i < TASK_SIZE; i++) {
        schedular.task_list[i].task = 0;
        schedular.task_list[i].ticks_interval = 0;
        schedular.task_list[i].ticks_previous = 0;
        schedular.task_count = 0;
    }
    if (get_ticks != 0) schedular.get_ticks = get_ticks;
}

void schedular_add(func_t task, uint32_t interval) {
    // find an empty task
    uint16_t i;
    for (i = 0; i < TASK_SIZE; i++) {
        // if an empty location is found, add the new task to it
        if (schedular.task_list[i].task == 0) {
            schedular.task_list[i].ticks_interval = interval;
            schedular.task_list[i].task = task;
            schedular.task_list[i].ticks_previous = schedular.get_ticks();
            schedular.task_count++;
            return;
        }
    }
}

void schedular_remove(func_t task) {
    // find the task
    uint16_t i;
    for (i = 0; i < TASK_SIZE; i++) {
        // a task is found
        // remove the task from the list
        if (schedular.task_list[i].task == task) {
            schedular.task_list[i].task = 0;
            schedular.task_count--;
            return;
        }
    }
}

void schedular_run(void) {
    uint16_t i = 0;
    uint32_t ticks = 0;
    // loop through all the task
    for (i = 0; i < schedular.task_count; i++) {
        // find a task
        if (schedular.task_list[i].task != 0) {
            // if task's timer is expired, run the task
            ticks = schedular.get_ticks();
            if ((ticks - schedular.task_list[i].ticks_previous) >= schedular.task_list[i].ticks_interval) {
                schedular.task_list[i].ticks_previous = ticks;
                // if task return 0, this mean the task want to remove
                // itself from the schedular
                if (schedular.task_list[i].task() == 0){
                    schedular_remove(schedular.task_list[i].task);
                }
            }
        }
    }
}

void schedular_print_task(void) {
    uint16_t i = 0;
    for (i = 0; i < TASK_SIZE; i++) {
        printf("task_%d addr        : %04X\n", i, (uint16_t) schedular.task_list[i].task);
        printf("task_%d interval    : %08X\n", i, (uint16_t) schedular.task_list[i].ticks_interval);
        printf("task_%d last        : %08X\n", i, (uint16_t) schedular.task_list[i].ticks_previous);
        printf("\n");
    }
}

uint16_t schedular_get_free_task(void) {
    return TASK_SIZE - schedular.task_count;
}