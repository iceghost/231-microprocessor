#ifndef __SEGMENT_DISPLAY_H
#define __SEGMENT_DISPLAY_H

#include "gpio_array.h"
#include "stm32f1xx_hal.h" // IWYU pragma: keep
#include <stdbool.h>

typedef struct {
  gpio_array_t gpios;
  uint16_t digit_to_gpios[10];
  size_t pos;
} segment_display_array_t;

void segment_display_array_init(segment_display_array_t *sda,
                                GPIO_TypeDef *port, uint16_t *pins, size_t len);
void segment_display_array_tick(segment_display_array_t *sda);
void segment_display_array_show(segment_display_array_t *sd_arr,
                                uint8_t *digits);

#endif
