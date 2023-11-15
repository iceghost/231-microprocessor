#ifndef __TRAFFIC_SIM_H
#define __TRAFFIC_SIM_H

#include "button.h"
#include "gpio_array.h"
#include "segment_display.h"
#include "software_timer.h"
#include <stdint.h>

enum {
  SOFTWARE_TIMER_1HZ = 0,
  SOFTWARE_TIMER_1TICK,
  SOFTWARE_TIMER_CONFIG_BLINK,
  SOFTWARE_TIMERS_COUNT,
};

typedef enum {
  LIGHT_RED_GREEN = 0,
  LIGHT_RED_YELLOW,
  LIGHT_GREEN_RED,
  LIGHT_YELLOW_RED,
} light_state_t;

typedef enum {
  COLOR_RED = 0,
  COLOR_YELLOW,
  COLOR_GREEN,
} color_t;

typedef enum {
  MODE_NORMAL,
  MODE_CONFIG,
} mode_t;

typedef struct {
  button_t buttons[3];
  segment_display_array_t sda;
  GPIO_TypeDef *led_sts_port;
  uint16_t led_sts_pin;
  
  gpio_array_t lights;
  uint16_t light_state_pins[4];

  software_timer_t sts[SOFTWARE_TIMERS_COUNT];

  uint8_t countdowns[3];

  mode_t mode;
  union {
    struct {
      light_state_t light_state;
      uint8_t v_countdown;
      uint8_t h_countdown;
    } normal;
    struct {
      color_t color;
      uint8_t countdown;
      bool blink;
    } config;
  };
} traffic_sim_t;

void traffic_sim_init(traffic_sim_t *ts);
void traffic_sim_main(traffic_sim_t *ts);

#endif
