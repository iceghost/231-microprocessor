#ifndef __TRAFFIC_SIM_H
#define __TRAFFIC_SIM_H

#include "button.h"
#include "software_timer.h"
#include <stdint.h>

enum {
  SOFTWARE_TIMER_1HZ = 0,
  SOFTWARE_TIMER_1TICK,
  SOFTWARE_TIMERS_COUNT,
};

typedef struct {
  button_t button;

  GPIO_TypeDef *led_sts_port;
  uint16_t led_sts_pin;

  software_timer_t sts[SOFTWARE_TIMERS_COUNT];
} traffic_sim_t;

void traffic_sim_init(traffic_sim_t *ts);
void traffic_sim_main(traffic_sim_t *ts);

#endif
