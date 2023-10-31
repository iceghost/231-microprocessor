#include "button.h"
#include "software_timer.h"
#include "stm32f1xx_hal.h" // IWYU pragma: keep

void button_tick(button_t *b) {
  bool reading = HAL_GPIO_ReadPin(b->port, b->pin);

  if (b->current != reading) {
    b->stable_tick = software_timer_tick;
    b->current = reading;
    return;
  }

  uint32_t current_tick = software_timer_tick;
  if (current_tick - b->stable_tick < b->debounce_threshold)
    return;

  b->known = b->current;
}
