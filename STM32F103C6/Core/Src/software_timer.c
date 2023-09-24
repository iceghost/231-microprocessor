#include "software_timer.h"

uint32_t software_timer_counts[SOFTWARE_TIMERS_COUNT];
bool software_timer_flags[SOFTWARE_TIMERS_COUNT];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  for (size_t i = 0; i < SOFTWARE_TIMERS_COUNT; i++) {
    software_timer_counts[i]--;
    if (software_timer_counts[i] == 0)
      software_timer_flags[i] = true;
  }
}
