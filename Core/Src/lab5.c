#include "adc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

char command_buf[4];
size_t command_len = 0;

enum {
  IDLE,
  SEND_DATA,
} state_kind;

void process(size_t begin, size_t end)
{
  static enum {
    NORMAL,
    WAIT_FOR_BANG,
  } state;

  for (size_t i = begin; i < end; ++i) {
    char ch;
    switch (ch = uart2_buf[i]) {
    case '!':
      state = NORMAL;
      command_len = 0;

      break;

    case '#':
      if (state == WAIT_FOR_BANG)
        continue;

      if (!strncmp(command_buf, "RST", 3)) {
        HAL_ADC_Start(&hadc1);
        state_kind = SEND_DATA;
      } else if (!strncmp(command_buf, "OK", 2)) {
        HAL_ADC_Stop(&hadc1);
        state_kind = IDLE;
      }

      break;

    default:

      if (state == WAIT_FOR_BANG)
        continue;

      if (command_len == sizeof(command_buf)) {
        state = WAIT_FOR_BANG;
        continue;
      }

      command_buf[command_len++] = ch;

      break;
    }
  }
}

void lab5_init()
{
}

void lab5_update()
{
  if (uart2_rx_done) {
    static size_t uart2_buf_last_pos = 0;
    size_t pos = uart2_buf_pos;

    if (pos > uart2_buf_last_pos)
      process(uart2_buf_last_pos, pos);
    else {
      /* uart buffer wrapped */
      process(uart2_buf_last_pos, sizeof(uart2_buf));
      process(0, pos);
    }

    uart2_rx_done = false;
    uart2_buf_last_pos = pos;
  }

  switch (state_kind) {
  case IDLE:
    break;
  case SEND_DATA: {
    static uint32_t last_sent_tick = 0;
    uint32_t tick = HAL_GetTick();

    if (tick - last_sent_tick > 3000) {
      static char buf[32];
      uint32_t value = HAL_ADC_GetValue(&hadc1);
      size_t len = snprintf(buf, sizeof(buf), "!ADC=%lu#", value);
      HAL_UART_Transmit(&huart2, (uint8_t *)buf, len, 50);
      last_sent_tick = tick;
    }

    break;
  }
  }
}
