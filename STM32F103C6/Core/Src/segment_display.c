#include "segment_display.h"

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
