#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "software_timer.h"
#include "stm32f1xx_hal.h" // IWYU pragma: keep
#include <stdbool.h>

typedef struct {
  bool known;
  bool current;
  uint32_t stable_tick;
  uint8_t debounce_threshold;

  GPIO_TypeDef *port;
  uint16_t pin;
} button_t;

inline void button_init(button_t *b, GPIO_TypeDef *port, uint16_t pin, uint8_t debounce_threshold) {
  b->port = port;
  b->pin = pin;
  b->debounce_threshold = debounce_threshold;
}

void button_tick(button_t *b);

inline bool button_is_pressed(button_t *b) {
  return b->known == GPIO_PIN_RESET;
}

inline bool button_is_longpressed(button_t *b, uint32_t duration) {
  return button_is_pressed(b) &&
         software_timer_tick - b->stable_tick >= duration;
}

#endif
