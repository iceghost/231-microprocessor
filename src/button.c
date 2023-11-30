#include "button.h"
#include <string.h>

void button_init(button_t *b, GPIO_TypeDef *port, uint16_t pin)
{
	memset(b, 0, sizeof(*b));
	b->port = port;
	b->pin = pin;
	b->reading = GPIO_PIN_SET;
	b->prev = GPIO_PIN_SET;
	b->current = GPIO_PIN_SET;
}

void button_read(button_t *b)
{
	b->reading = HAL_GPIO_ReadPin(b->port, b->pin);
}

void button_tick(button_t *b)
{
	if (b->current != b->reading) {
		b->current = b->reading;
		b->stable_ticks = 0;
		return;
	}

	if (++b->stable_ticks < DEBOUNCE_THRESHOLD)
		return;

	b->stable_ticks = DEBOUNCE_THRESHOLD;
}

void button_update(button_t *b)
{
	if (b->stable_ticks < DEBOUNCE_THRESHOLD)
		return;

	if (b->prev != b->current) {
		if (b->prev == GPIO_PIN_SET)
			b->is_pressed = true;
		else
			b->is_depressed = true;

		b->prev = b->current;
	}
}
