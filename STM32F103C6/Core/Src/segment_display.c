#include "segment_display.h"
#include "software_timer.h"

void segment_display_show_num(segment_display_t *sd, uint8_t num) {
  bool segment_states[10][7] = {
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
  uint16_t on_pins = 0;
  uint16_t off_pins = 0;
  for (uint8_t i = 0; i < 7; i++) {
    if (segment_states[num][i])
      on_pins |= sd->pins[i];
    else
      off_pins |= sd->pins[i];
  }
  HAL_GPIO_WritePin(sd->port, on_pins, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(sd->port, off_pins, GPIO_PIN_SET);
}

void segment_display_array_show(segment_display_array_t *sd_arr,
                                uint8_t *digits, size_t size) {
  uint16_t all_pins = 0;
  uint16_t on_pins = sd_arr->en_pins[sd_arr->active_pin_i];
  for (size_t i = 0; i < size; i++)
    all_pins |= sd_arr->en_pins[i];

  HAL_GPIO_WritePin(sd_arr->en_port, all_pins & ~on_pins, GPIO_PIN_SET);

  segment_display_show_num(&sd_arr->sd, digits[sd_arr->active_pin_i]);

  if (software_timer_flags[sd_arr->timer_i]) {
    sd_arr->active_pin_i += 1;
    sd_arr->active_pin_i %= size;
    software_timer_reset(sd_arr->timer_i, sd_arr->t_timer);
  }

  HAL_GPIO_WritePin(sd_arr->en_port, on_pins,
                    GPIO_PIN_RESET);
}
