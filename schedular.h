/* 
 * File:   schedular.h
 * Author: james
 *
 * Created on 11 December 2018, 3:27 PM
 */

#ifndef SCHEDULAR_H
#define	SCHEDULAR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

    // define the maximum number of task that is available
#define TASK_SIZE     (10U)

    // function pointer use in task
    typedef uint16_t (*func_t)(void);
    typedef uint32_t(*get_ticks_t)(void);

    // task struct for use in schedular

    typedef struct {
        uint32_t ticks_interval; // task interval, 
        uint32_t ticks_previous; // previous ticks
        func_t task; // function pointer
    } task_t;

    typedef struct {
        // will be point to external getTicks() function
        get_ticks_t get_ticks; 
        
        // an task array to store all the task, and their params
        // and the task schedular will read this list the run
        // task according their params
        task_t task_list[TASK_SIZE];
        
        // the number of tasks
        uint16_t task_count;
    } schedular_t;

    extern schedular_t schedular;

    // initial schedular, this must run first
    // @param task      - pointer to a task than return system ticks
    void schedular_init(get_ticks_t get_ticks);

    // add a task to the schedular
    // @param task      - the task to add
    // @param interval  - how often the task need to run 
    void schedular_add(func_t task, uint32_t interval);

    // remove a task from the schedular
    // @param task      - the task to be removed
    void schedular_remove(func_t task);

    // this run the schedular, this should be in the main loop
    void schedular_run(void);

    void schedular_print_task(void);

    uint16_t schedular_get_free_task(void);



#ifdef	__cplusplus
}
#endif

#endif	/* SCHEDULAR_H */
