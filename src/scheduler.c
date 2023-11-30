#include "scheduler.h"
#include <stddef.h>

scheduler_task_t scheduler_tasks[SCHEDULER_MAX_TASKS];

bool sch_delete_task(uint16_t id);

bool sch_add_task(void (*callback)(), uint16_t delay, uint16_t period)
{
	uint16_t i = 0;

	while (i < SCHEDULER_MAX_TASKS && scheduler_tasks[i].callback) {
		i++;
	}
	if (i == SCHEDULER_MAX_TASKS)
		return 0;

	scheduler_tasks[i].callback = callback;
	scheduler_tasks[i].delay = delay;
	scheduler_tasks[i].period = period;
	scheduler_tasks[i].run_me = 0;

	return 1;
}

void sch_update(void)
{
	for (uint16_t i = 0; i < SCHEDULER_MAX_TASKS; i++) {
		if (!scheduler_tasks[i].callback)
			continue;

		if (scheduler_tasks[i].delay > 0) {
			scheduler_tasks[i].delay--;
			continue;
		}

		scheduler_tasks[i].run_me++;

		if (scheduler_tasks[i].period)
			scheduler_tasks[i].delay = scheduler_tasks[i].period;
	}
}

void sch_dispatch(void)
{
	for (uint8_t i = 0; i < SCHEDULER_MAX_TASKS; i++) {
		if (scheduler_tasks[i].run_me && scheduler_tasks[i].callback) {
			scheduler_tasks[i].callback();
			scheduler_tasks[i].run_me--;
			if (scheduler_tasks[i].period == 0)
				sch_delete_task(i);
		}
	}
}

bool sch_delete_task(uint16_t id)
{
	if (id >= SCHEDULER_MAX_TASKS)
		return 0;
	scheduler_tasks[id].callback = NULL;
	return 1;
}
