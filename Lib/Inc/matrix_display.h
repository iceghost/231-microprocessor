#ifndef __MATRIX_DISPLAY_H
#define __MATRIX_DISPLAY_H

#include "main.h"

#include "software_timer.h"

typedef struct {
  GPIO_TypeDef *en_port;
  uint16_t en_pins[8];
  GPIO_TypeDef *row_port;
  uint16_t row_pins[8];
  uint8_t active_en_i;
  software_timer_handle_t ti;
  software_timer_handle_t animate_ti;
} matrix_display_t;

typedef struct {
  size_t i;
  uint8_t lshift;
} matrix_display_state_t;

void matrix_display_show(matrix_display_t *md, uint8_t *matrix);
void matrix_display_animate(matrix_display_t *md, const char *str, size_t size,
                            matrix_display_state_t *state);

#endif
