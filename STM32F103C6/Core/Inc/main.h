/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HAND_0_Pin GPIO_PIN_4
#define HAND_0_GPIO_Port GPIOA
#define HAND_1_Pin GPIO_PIN_5
#define HAND_1_GPIO_Port GPIOA
#define HAND_2_Pin GPIO_PIN_6
#define HAND_2_GPIO_Port GPIOA
#define HAND_3_Pin GPIO_PIN_7
#define HAND_3_GPIO_Port GPIOA
#define HAND_4_Pin GPIO_PIN_8
#define HAND_4_GPIO_Port GPIOA
#define HAND_5_Pin GPIO_PIN_9
#define HAND_5_GPIO_Port GPIOA
#define HAND_6_Pin GPIO_PIN_10
#define HAND_6_GPIO_Port GPIOA
#define HAND_7_Pin GPIO_PIN_11
#define HAND_7_GPIO_Port GPIOA
#define HAND_8_Pin GPIO_PIN_12
#define HAND_8_GPIO_Port GPIOA
#define HAND_9_Pin GPIO_PIN_13
#define HAND_9_GPIO_Port GPIOA
#define HAND_10_Pin GPIO_PIN_14
#define HAND_10_GPIO_Port GPIOA
#define HAND_11_Pin GPIO_PIN_15
#define HAND_11_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define SOFTWARE_TIMERS_COUNT 2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
