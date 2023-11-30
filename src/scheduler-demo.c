#include "stm32f1xx_hal.h"
#include <stdio.h>

void setup()
{
}

void loop()
{
	printf("hello qemu!\n");
	HAL_Delay(1000);
}
