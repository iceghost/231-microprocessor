/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <assert.h>
#include <string.h>
/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{
  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  if (uartHandle->Instance == USART2) {
    /* USER CODE BEGIN USART2_MspInit 0 */

    /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    /* USER CODE BEGIN USART2_MspInit 1 */

    /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{
  if (uartHandle->Instance == USART2) {
    /* USER CODE BEGIN USART2_MspDeInit 0 */

    /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
    /* USER CODE BEGIN USART2_MspDeInit 1 */

    /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint8_t temp;
uint8_t buf[2][1024];
uint8_t write_buf = 0;
size_t read_buf_size = 0;
size_t pos = 0;

enum {
  HASH,
  BANG,
} expecting_char = BANG;

void uart2_init(void)
{
  HAL_UART_Receive_IT(&huart2, &temp, 1);
}

int uart2_read(uint8_t *dest, size_t size, size_t *read_len)
{
  if (read_buf_size == 0)
    return UART_BUFFER_NOT_READY;

  *read_len = read_buf_size;

  if (read_buf_size > size)
    return UART_DEST_TOO_SMALL;

  memcpy(dest, buf[write_buf ^ 1], read_buf_size);
  read_buf_size = 0;

  return UART_OK;
}

int uart2_write(uint8_t *buf, size_t size)
{
  HAL_UART_Transmit(&huart2, buf, size, 50);

  return UART_OK;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  uart2_write(&temp, 1);

  assert(huart == &huart2);

  switch (expecting_char) {
  case BANG:
    /* wait for start of new command */
    if (temp == '!') {
      expecting_char = HASH;
      pos = 0;
      break;
    }

    break;

  case HASH:
    if (temp == '#') {
      write_buf ^= 1;
      expecting_char = BANG;
      read_buf_size = pos;
      break;
    }

    buf[write_buf][pos] = temp;

    if (++pos == sizeof(buf[0])) {
      /* buffer overflow, wait for start of new command */
      expecting_char = BANG;
      break;
    }

    break;
  }

  HAL_UART_Receive_IT(&huart2, &temp, 1);
}
/* USER CODE END 1 */
