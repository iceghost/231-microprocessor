#include "traffic_sim.h"
#include "button.h"
#include "main.h"
#include "software_timer.h"

void traffic_sim_init(traffic_sim_t *ts) {
  button_init(&ts->button, BUTTON_GPIO_Port, BUTTON_Pin, 3);
  software_timer_init(&ts->sts[SOFTWARE_TIMER_1HZ], 10);
  software_timer_init(&ts->sts[SOFTWARE_TIMER_1TICK], 1);
  software_timers_start(ts->sts, SOFTWARE_TIMERS_COUNT);
}

void traffic_sim_main(traffic_sim_t *ts) {
  software_timer_if_flagged(&ts->sts[SOFTWARE_TIMER_1HZ]) {
    HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
  }

  // software_timer_if_flagged(&ts->sts[SOFTWARE_TIMER_1TICK]) {
  //   button_tick(&ts->button);
  // }
}
