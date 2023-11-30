#include "button.h"
#include "stm32f1xx_hal_gpio.h"
#include <assert.h>
#include <stdio.h>

button_t button;

void init()
{
	button_init(&button, GPIOA, GPIO_PIN_5);
}

void update()
{
	button_update(&button);

	assert(!button_is_pressed(&button));

	button.reading = 0;

	button_tick(&button);
	button_update(&button);

	assert(!button_is_pressed(&button));

	for (int i = 0; i < 3; i++) {
		button_tick(&button);
		button_update(&button);
	}

	assert(button_is_pressed(&button));

	printf("test ok");

	while (true) {
	}
}

void tick()
{
}
