#include "matrix_display.h"

void matrix_display_show(matrix_display_t *md, uint8_t *matrix) {
  uint16_t on_en = md->en_pins[md->active_en_i];
  uint16_t all_ens = 0;
  for (size_t i = 0; i < 8; i++)
    all_ens |= md->en_pins[i];

  HAL_GPIO_WritePin(md->en_port, all_ens & ~on_en, GPIO_PIN_SET);  

  uint16_t on_row = 0;
  uint16_t all_rows = 0;
  for (size_t i = 0; i < 8; i++) {
    all_rows |= md->row_pins[i];
    if (matrix[i] & (1 << md->active_en_i))
      on_row |= md->row_pins[i];
  }
  
  HAL_GPIO_WritePin(md->row_port, all_rows & ~on_row, GPIO_PIN_SET);
  HAL_GPIO_WritePin(md->row_port, on_row, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(md->en_port, on_en, GPIO_PIN_RESET);  

  IF_SOFTWARE_TIMER_ITEM_FLAGGED(&md->ti) {
    md->active_en_i += 1;
    md->active_en_i %= 8;
  }
}
