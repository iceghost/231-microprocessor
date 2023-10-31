#include "segment_display.h"
#include <stdbool.h>

void segment_display_show(segment_display_t *sd, uint8_t num) {
  gpio_array_write(&sd->gpios, sd->digit_to_gpios[num]);
}

void segment_display_array_tick(segment_display_array_t *sda) {
  sda->pos += 1;
  sda->pos %= sda->en_gpios.len;
}

void segment_display_array_show(segment_display_array_t *sda, uint8_t *digits) {
  gpio_array_set(&sda->en_gpios, sda->en_gpios.mask);
  segment_display_show(&sda->sd, digits[sda->pos]);
  gpio_array_reset(&sda->en_gpios, sda->en_gpios.pins[sda->pos]);
}
