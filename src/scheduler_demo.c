#include "scheduler.h"
#include "stm32f1xx.h" // IWYU pragma: keep
#include <stdbool.h>
#include <stdio.h>

scheduler_task_t tasks[4];

uint32_t t;

void print_timestamp(void *data)
{
	const char *text = data;
	printf("[%05d] %s\n", HAL_GetTick(), text);
}

void scheduler_dump()
{
	scheduler_task_t *tk = GLOBAL_SCHEDULER.head;
	while (tk) {
		printf("addr=%p delay=%d\n", tk, tk->delay);
		tk = tk->next;
	}
}

void init()
{
	scheduler_init();
	scheduler_task_init(&tasks[0], 1, false, print_timestamp, "10ms timer");
	scheduler_add_task(&tasks[0]);
	scheduler_task_init(&tasks[1], 25, false, print_timestamp,
			    "250ms timer");
	scheduler_add_task(&tasks[1]);
	scheduler_task_init(&tasks[2], 50, false, print_timestamp,
			    "500ms timer");
	scheduler_add_task(&tasks[2]);
	scheduler_task_init(&tasks[3], 69, true, print_timestamp,
			    "1 shot timer");
	scheduler_add_task(&tasks[3]);
}

void update()
{
	scheduler_update();
}

void tick()
{
	scheduler_tick();
}
