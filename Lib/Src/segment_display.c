#include "segment_display.h"
#include "gpio_array.h"
#include <stdbool.h>
#include <string.h>

void segment_display_array_init(segment_display_array_t *sda,
                                GPIO_TypeDef *port, uint16_t *pins,
                                size_t len) {
  gpio_array_init(&sda->gpios, port, pins, len);
  memset(sda->digit_to_gpios, 0, 20);
  sda->pos = 7;

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
        sda->digit_to_gpios[num] |= sda->gpios.pins[i];
}

void segment_display_array_tick(segment_display_array_t *sda) {
  sda->pos += 1;
  if (sda->pos == sda->gpios.len)
    sda->pos = 7;
}

void segment_display_array_show(segment_display_array_t *sda, uint8_t *digits) {
  uint16_t write_pins = sda->digit_to_gpios[digits[sda->pos - 7]];
  write_pins |= sda->gpios.pins[sda->pos];
  gpio_array_write(&sda->gpios, write_pins);
}
