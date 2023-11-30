#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f1xx.h" // IWYU pragma: keep
#include <stdbool.h>
#include <stdint.h>

#define DEBOUNCE_THRESHOLD 3

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;

	// input states
	bool reading;

	// update
	bool prev;
	bool current;
	uint8_t stable_ticks;
	bool is_pressed;
	bool is_depressed;
} button_t;

void button_init(button_t *b, GPIO_TypeDef *port, uint16_t pin);
void button_read(button_t *b);
void button_tick(button_t *b);
void button_update(button_t *b);

inline bool button_is_pressed(button_t *b)
{
	bool tmp = b->is_pressed;
	b->is_pressed = false;
	return tmp;
}

inline bool button_is_depressed(button_t *b)
{
	bool tmp = b->is_depressed;
	b->is_depressed = false;
	return tmp;
}

#endif
