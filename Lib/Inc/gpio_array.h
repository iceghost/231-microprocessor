#ifndef __GPIO_ARRAY_H
#define __GPIO_ARRAY_H

#include "stm32f1xx_hal.h" // IWYU pragma: keep
#include <stddef.h>
#include <stdint.h>

typedef struct {
  GPIO_TypeDef *port;
  uint16_t *pins;
  size_t len;
  uint16_t mask;
} gpio_array_t;

void gpio_array_init(gpio_array_t *ga, GPIO_TypeDef *port, uint16_t *pins,
                     size_t len);

inline void gpio_array_set(gpio_array_t *ga, uint16_t gpios) {
  HAL_GPIO_WritePin(ga->port, gpios, GPIO_PIN_SET);
}

inline void gpio_array_reset(gpio_array_t *ga, uint16_t gpios) {
  HAL_GPIO_WritePin(ga->port, gpios, GPIO_PIN_RESET);
}

inline void gpio_array_write(gpio_array_t *ga, uint16_t gpios) {
  gpio_array_reset(ga, gpios);
  gpio_array_set(ga, ga->mask & ~gpios);
}

#endif
