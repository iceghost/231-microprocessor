#include "traffic_sim.h"
#include "button.h"
#include "gpio_array.h"
#include "main.h"
#include "segment_display.h"
#include "software_timer.h"

#define DEBOUNCE_THRESHOLD 3
#define LONGPRESS_THRESHOLD 10

// #define CONFIG_BLINK_DURATION (0.1 * software_timer_ticks_per_second)
#define CONFIG_BLINK_DURATION 3

void traffic_sim_set_light(traffic_sim_t *ts, light_state_t ls) {
  ts->normal.light_state = ls;
  uint8_t yellow_countdown =
      ts->countdowns[COLOR_RED] - ts->countdowns[COLOR_GREEN];

  switch (ls) {
  case LIGHT_RED_GREEN:
    ts->normal.v_countdown = ts->countdowns[COLOR_RED];
    ts->normal.h_countdown = ts->countdowns[COLOR_GREEN];
    break;
  case LIGHT_RED_YELLOW:
    ts->normal.h_countdown = yellow_countdown;
    break;
  case LIGHT_GREEN_RED:
    ts->normal.v_countdown = ts->countdowns[COLOR_GREEN];
    ts->normal.h_countdown = ts->countdowns[COLOR_RED];
    break;
  case LIGHT_YELLOW_RED:
    ts->normal.v_countdown = yellow_countdown;
    break;
  }
}

void traffic_sim_init(traffic_sim_t *ts) {
  // buttons

  static uint16_t BUTTON_PINS[] = {
      BUTTON_1_Pin,
      BUTTON_2_Pin,
      BUTTON_3_Pin,
  };
  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]); i++) {
    button_init(&ts->buttons[i], GPIOA, BUTTON_PINS[i], DEBOUNCE_THRESHOLD,
                LONGPRESS_THRESHOLD);
  }

  // 7-segments pins and enable pins

  static uint16_t SEG_PINS[] = {
      SEG_A_Pin,    SEG_B_Pin,    SEG_C_Pin,    SEG_D_Pin,
      SEG_E_Pin,    SEG_F_Pin,    SEG_G_Pin, //
      SEG_EN_1_Pin, SEG_EN_2_Pin, SEG_EN_3_Pin, SEG_EN_4_Pin,
  };
  segment_display_array_init(&ts->sda, GPIOB, SEG_PINS,
                             sizeof(SEG_PINS) / sizeof(SEG_PINS[0]));

  // all the lights, vertical and horizontal

  static uint16_t LIGHT_PINS[] = {
      LED_V_RED_Pin,   LED_V_YELLOW_Pin,
      LED_V_GREEN_Pin, //
      LED_H_RED_Pin,   LED_H_YELLOW_Pin,
      LED_H_GREEN_Pin, //
  };
  gpio_array_init(&ts->lights, GPIOA, LIGHT_PINS,
                  sizeof(LIGHT_PINS) / sizeof(LIGHT_PINS[0]));
  ts->light_state_pins[LIGHT_RED_GREEN] =
      LIGHT_PINS[COLOR_RED] | LIGHT_PINS[COLOR_GREEN + 3];
  ts->light_state_pins[LIGHT_RED_YELLOW] =
      LIGHT_PINS[COLOR_RED] | LIGHT_PINS[COLOR_YELLOW + 3];
  ts->light_state_pins[LIGHT_GREEN_RED] =
      LIGHT_PINS[COLOR_GREEN] | LIGHT_PINS[COLOR_RED + 3];
  ts->light_state_pins[LIGHT_YELLOW_RED] =
      LIGHT_PINS[COLOR_YELLOW] | LIGHT_PINS[COLOR_RED + 3];

  // software timer

  software_timers_init();
  software_timer_init(&ts->sts[SOFTWARE_TIMER_1HZ],
                      0.1 * software_timer_ticks_per_second);
  software_timer_init(&ts->sts[SOFTWARE_TIMER_1TICK], 1);
  software_timer_init(&ts->sts[SOFTWARE_TIMER_CONFIG_BLINK],
                      CONFIG_BLINK_DURATION);
  software_timers_start(ts->sts, SOFTWARE_TIMERS_COUNT);

  // default value for countdown
  ts->countdowns[COLOR_RED] = 5;
  ts->countdowns[COLOR_YELLOW] = 2;
  ts->countdowns[COLOR_GREEN] = 3;

  // initialize state

  ts->mode = MODE_NORMAL;

  traffic_sim_set_light(ts, LIGHT_RED_GREEN);
}

void traffic_sim_fsm(traffic_sim_t *ts) {
  uint8_t segment_buf[4];

  switch (ts->mode) {
  case MODE_NORMAL:
    segment_buf[0] = ts->normal.v_countdown / 10;
    segment_buf[1] = ts->normal.v_countdown % 10;
    segment_buf[2] = ts->normal.h_countdown / 10;
    segment_buf[3] = ts->normal.h_countdown % 10;
    segment_display_array_show(&ts->sda, segment_buf);
    gpio_array_write(&ts->lights, ts->light_state_pins[ts->normal.light_state]);

    button_on_pressed(&ts->buttons[0]) {
      ts->mode = MODE_CONFIG;
      ts->config.color = COLOR_RED;
      ts->config.countdown = ts->countdowns[COLOR_RED];
    }

    software_timer_if_flagged(&ts->sts[SOFTWARE_TIMER_1HZ]) {
      HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
      ts->normal.v_countdown -= 1;
      ts->normal.h_countdown -= 1;
      if (ts->normal.v_countdown == 0 || ts->normal.h_countdown == 0)
        traffic_sim_set_light(ts, (ts->normal.light_state + 1) % 4);
    }
    break;

  case MODE_CONFIG:
    segment_buf[0] = ts->config.countdown / 10;
    segment_buf[1] = ts->config.countdown % 10;
    segment_buf[2] = 0;
    // RED -> 2, YELLOW -> 3, GREEN -> 4
    segment_buf[3] = ts->config.color + 2;
    segment_display_array_show(&ts->sda, segment_buf);

    if (ts->config.blink)
      gpio_array_write(&ts->lights, ts->lights.pins[ts->config.color] |
                                        ts->lights.pins[ts->config.color + 3]);
    else
      gpio_array_write(&ts->lights, 0);

    button_on_pressed(&ts->buttons[0]) {
      switch (ts->config.color) {
      case COLOR_RED:
        ts->config.color = COLOR_GREEN;
        ts->config.countdown = ts->countdowns[COLOR_GREEN];
        break;
      default:
        ts->mode = MODE_NORMAL;
        traffic_sim_set_light(ts, LIGHT_RED_GREEN);
        break;
      }
    }

    button_on_pressed(&ts->buttons[1]) {
      ts->config.countdown += 1;
      uint8_t countdown_min[] = {
          [COLOR_RED] = 2,
          [COLOR_GREEN] = 1,
      };
      uint8_t countdown_max[] = {
          [COLOR_RED] = 99,
          [COLOR_GREEN] = ts->countdowns[COLOR_RED] - 1,
      };
      if (ts->config.countdown > countdown_max[ts->config.color])
        ts->config.countdown = countdown_min[ts->config.color];
    }

    button_on_pressed(&ts->buttons[2]) {
      ts->countdowns[COLOR_RED] = ts->config.countdown;
    }

    software_timer_if_flagged(&ts->sts[SOFTWARE_TIMER_1HZ]) {
      HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
      ts->config.blink = !ts->config.blink;
    }

    software_timer_if_flagged(&ts->sts[SOFTWARE_TIMER_CONFIG_BLINK]) {
    }
    break;
  }
}

void traffic_sim_main(traffic_sim_t *ts) {
  // can run this without 1 tick delay,
  // but proteus does not like that
  traffic_sim_fsm(ts);

  software_timer_if_flagged(&ts->sts[SOFTWARE_TIMER_1TICK]) {
    button_tick(&ts->buttons[0]);
    button_tick(&ts->buttons[1]);
    button_tick(&ts->buttons[2]);
    segment_display_array_tick(&ts->sda);
  }
}
