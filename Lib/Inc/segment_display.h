#ifndef __SEGMENT_DISPLAY_H
#define __SEGMENT_DISPLAY_H

#include "gpio_array.h"
#include "stm32f1xx_hal.h" // IWYU pragma: keep
#include <stdbool.h>
#include <string.h>

typedef struct {
  gpio_array_t gpios;
  uint16_t digit_to_gpios[10];
} segment_display_t;

typedef struct {
  segment_display_t sd;
  gpio_array_t en_gpios;
  size_t pos;
} segment_display_array_t;

void segment_display_init(segment_display_t *sd, GPIO_TypeDef *port,
                          uint16_t *pins) {
  gpio_array_init(&sd->gpios, port, pins, 7);
  memset(sd->digit_to_gpios, 0, 20);

  static bool segment_states[10][7] = {
      {1, 1, 1, 1, 1, 1, 0}, /* */
      {0, 1, 1, 0, 0, 0, 0}, /* */
      {1, 1, 0, 1, 1, 0, 1}, /* */
      {1, 1, 1, 1, 0, 0, 1}, /* */
      {0, 1, 1, 0, 0, 1, 1}, /* */
      {1, 0, 1, 1, 0, 1, 1}, /* */
      {1, 0, 1, 1, 1, 1, 1}, /* */
      {1, 1, 1, 0, 0, 0, 0}, /* */
      {1, 1, 1, 1, 1, 1, 1}, /* */
      {1, 1, 1, 1, 0, 1, 1}, /* */
  };
  for (uint8_t num = 0; num < 10; num++)
    for (uint8_t i = 0; i < 7; i++)
      if (segment_states[num][i])
        sd->digit_to_gpios[num] |= sd->gpios.pins[i];
}
void segment_display_show(segment_display_t *sd, uint8_t num);
void segment_display_array_tick(segment_display_array_t *sda);
void segment_display_array_show(segment_display_array_t *sd_arr,
                                uint8_t *digits);

#endif
