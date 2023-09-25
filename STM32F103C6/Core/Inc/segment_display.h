#ifndef __SEGMENT_DISPLAY_H
#define __SEGMENT_DISPLAY_H

#include "main.h"

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pins[7];
} segment_display_t;

#define INIT_SEGMENT_DISPLAY_PINS(name, prefix)                                \
  do {                                                                         \
    name.pins[0] = prefix##_A_Pin;                                        \
    name.pins[1] = prefix##_B_Pin;                                        \
    name.pins[2] = prefix##_C_Pin;                                        \
    name.pins[3] = prefix##_D_Pin;                                        \
    name.pins[4] = prefix##_E_Pin;                                        \
    name.pins[5] = prefix##_F_Pin;                                        \
    name.pins[6] = prefix##_G_Pin;                                        \
  } while (0)

void segment_display_show_num(segment_display_t *sd, uint8_t num);

#endif
