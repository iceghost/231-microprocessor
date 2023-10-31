#include "gpio_array.h"
#include "stm32f1xx_hal.h" // IWYU pragma: keep

void gpio_array_init(gpio_array_t *ga, GPIO_TypeDef *port, uint16_t *pins,
                     size_t len) {
  ga->port = port;
  ga->pins = pins;
  ga->len = len;
  ga->mask = 0;

  for (size_t i = 0; i < ga->len; i++)
    ga->mask |= ga->pins[i];
}
