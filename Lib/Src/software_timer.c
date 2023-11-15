#include "software_timer.h"
#include "stm32f1xx_hal.h" // IWYU pragma: keep

#define BASE_CLK 64000000

extern TIM_HandleTypeDef htim2;
static software_timer_t *software_timers;
static size_t software_timers_count;

uint32_t software_timer_tick;
uint32_t software_timer_ticks_per_second;

void software_timers_init() {
  software_timer_tick = 0;
  software_timer_ticks_per_second = BASE_CLK;
  software_timer_ticks_per_second /= htim2.Init.Prescaler + 1;
  software_timer_ticks_per_second /= htim2.Init.Period + 1;
}

void software_timers_start(software_timer_t *sts, size_t count) {
  software_timers = sts;
  software_timers_count = count;
  HAL_TIM_Base_Start_IT(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  software_timer_tick += 1;
  for (size_t i = 0; i < software_timers_count; i++) {
    software_timer_t *st = &software_timers[i];
    if (software_timer_tick - st->start >= st->interval)
      st->flag = true;
  }
}
