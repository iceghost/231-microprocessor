#include "scheduler.h"
#include <stddef.h>

scheduler_t GLOBAL_SCHEDULER;

void scheduler_init()
{
	GLOBAL_SCHEDULER.head = NULL;
	GLOBAL_SCHEDULER.due_oneshot_head = NULL;
}

static void scheduler_dispatch()
{
	scheduler_task_t *tk = GLOBAL_SCHEDULER.head;
	while (tk) {
		if (tk->due_runs) {
			tk->due_runs--;
			tk->callback(tk->data);
		}
		tk = tk->next;
	}

	while ((tk = GLOBAL_SCHEDULER.due_oneshot_head)) {
		tk->callback(tk->data);
		GLOBAL_SCHEDULER.due_oneshot_head = tk->next;
	}
}

void scheduler_update()
{
	scheduler_dispatch();
}

void scheduler_tick()
{
	scheduler_task_t *tk = GLOBAL_SCHEDULER.head;
	if (!tk)
		return;

	if (tk->delay > 0) {
		tk->delay--;
	}

	scheduler_task_t *due_list = NULL;

	while (tk && tk->delay == 0) {
		tk->due_runs++;

		// pop from scheduler
		GLOBAL_SCHEDULER.head = tk->next;
		// push to due list if interval
		if (tk->period) {
			tk->delay = tk->period;
			tk->next = due_list;
			due_list = tk;
		} else {
			tk->next = GLOBAL_SCHEDULER.due_oneshot_head;
			GLOBAL_SCHEDULER.due_oneshot_head = tk;
		}

		tk = GLOBAL_SCHEDULER.head;
	}

	while (due_list) {
		scheduler_task_t *next = due_list->next;
		scheduler_add_task(due_list);
		due_list = next;
	}
}

void scheduler_add_task(scheduler_task_t *task)
{
	scheduler_task_t **head_link = &GLOBAL_SCHEDULER.head;

	while (true) {
		scheduler_task_t *head = *head_link;
		if (!head) {
			*head_link = task;
			task->next = NULL;
			return;
		}

		if (task->delay <= head->delay) {
			head->delay -= task->delay;
			task->next = head;
			*head_link = task;
			return;
		}

		task->delay -= head->delay;
		head_link = &head->next;
	}
}

void scheduler_task_init(scheduler_task_t *task, uint32_t delay, bool oneshot,
			 void (*callback)(void *), void *data)
{
	task->delay = delay;
	task->period = oneshot ? 0 : delay;
	task->callback = callback;
	task->data = data;
	task->due_runs = 0;
	task->next = NULL;
}
