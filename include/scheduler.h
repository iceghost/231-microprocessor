#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	// Pointer to the task
	void (*callback)(void);
	// Delay (ticks) until the function will (next) be run
	uint32_t delay;
	// Interval (ticks) between subsequent runs
	uint32_t period;
	// Incremented (by scheduler) when task is due to execute
	uint8_t run_me;
	uint32_t task_id;
} scheduler_task_t;

#define SCHEDULER_MAX_TASKS 40

extern scheduler_task_t scheduler_tasks[SCHEDULER_MAX_TASKS];

bool scheduler_add_task(void (*ptask)(void), uint16_t delay, uint16_t period);
void scheduler_update(void);
bool scheduler_dispatch(void);

#endif
