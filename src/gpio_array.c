#include "gpio_array.h"
#include "stm32f1xx_hal.h" // IWYU pragma: keep
#include "stm32f1xx_hal_gpio.h"

void gpio_array_init(gpio_array_t *ga, GPIO_TypeDef *port, uint16_t *pins,
		     size_t len)
{
	ga->port = port;
	ga->pins = pins;
	ga->len = len;
	ga->mask = 0;

	for (size_t i = 0; i < ga->len; i++)
		ga->mask |= ga->pins[i];
}

void gpio_array_set(gpio_array_t *ga, uint16_t gpios)
{
	ga->set_mask |= gpios;
}

void gpio_array_reset(gpio_array_t *ga, uint16_t gpios)
{
	ga->set_mask &= ~gpios;
}

void gpio_array_set_only(gpio_array_t *ga, uint16_t gpios)
{
	gpio_array_reset(ga, gpios);
	gpio_array_set(ga, ga->mask & ~gpios);
}

void gpio_array_update(gpio_array_t *ga)
{
	HAL_GPIO_WritePin(ga->port, ga->set_mask, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ga->port, ga->mask & ~ga->set_mask, GPIO_PIN_RESET);
}
