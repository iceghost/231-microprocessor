#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct scheduler_task {
	uint32_t delay;
	uint32_t period;
	uint8_t due_runs;

	void (*callback)(void *);
	void *data;

	struct scheduler_task *next;
} scheduler_task_t;

typedef struct {
	scheduler_task_t *head;
	scheduler_task_t *due_oneshot_head;
} scheduler_t;

extern scheduler_t GLOBAL_SCHEDULER;

void scheduler_init();
void scheduler_tick();
void scheduler_update();

void scheduler_task_init(scheduler_task_t *task, uint32_t delay, bool oneshot,
			 void (*callback)(void *), void *data);
void scheduler_add_task(scheduler_task_t *task);

#endif
