#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f1xx_hal.h" // IWYU pragma: keep
#include <stdbool.h>

typedef struct {
  bool known;
  bool current;
  uint32_t stable_tick;
  uint32_t pressed_tick;
  uint8_t debounce_threshold;
  uint32_t longpress_threshold;

  bool is_pressed;
  bool is_released;
  bool is_longpressed;

  GPIO_TypeDef *port;
  uint16_t pin;
} button_t;

inline void button_init(button_t *b, GPIO_TypeDef *port, uint16_t pin,
                        uint8_t debounce_threshold,
                        uint32_t longpress_threshold) {
  b->port = port;
  b->pin = pin;
  b->debounce_threshold = debounce_threshold;
  b->longpress_threshold = longpress_threshold;
}

void button_tick(button_t *b);

#define button_on_pressed(b) for (; (b)->is_pressed; (b)->is_pressed = false)

#define button_on_released(b) for (; (b)->is_released; (b)->is_released = false)

#define button_on_longpressed(b)                                               \
  for (; (b)->is_longpressed; (b)->is_longpressed = false)

#endif
