#include "stm32f1xx_hal.h"
#include "tim.h"
#include <stdbool.h>
#include <stdio.h>

int i;

void init()
{
	HAL_TIM_Base_Start_IT(&htim2);
}

void update()
{
	printf("%d\n", i);
	HAL_Delay(100);
}

void tick()
{
	i++;
}
