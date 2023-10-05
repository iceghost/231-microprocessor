#ifndef __SEGMENT_DISPLAY_H
#define __SEGMENT_DISPLAY_H

#include "main.h"

#include "software_timer.h"

#ifndef MAX_SEGMENT_DISPLAY_ARRAY_SIZE
#define MAX_SEGMENT_DISPLAY_ARRAY_SIZE 10
#endif

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pins[7];
} segment_display_t;

typedef struct {
  segment_display_t sd;
  GPIO_TypeDef *en_port;
  uint16_t en_pins[MAX_SEGMENT_DISPLAY_ARRAY_SIZE];
  size_t active_pin_i;
  software_timer_handle_t ti;
} segment_display_array_t;

#define INIT_SEGMENT_DISPLAY(name, init_port, prefix)                          \
  do {                                                                         \
    name.port = init_port;                                                     \
    name.pins[0] = prefix##_A_Pin;                                             \
    name.pins[1] = prefix##_B_Pin;                                             \
    name.pins[2] = prefix##_C_Pin;                                             \
    name.pins[3] = prefix##_D_Pin;                                             \
    name.pins[4] = prefix##_E_Pin;                                             \
    name.pins[5] = prefix##_F_Pin;                                             \
    name.pins[6] = prefix##_G_Pin;                                             \
  } while (0)

void segment_display_show_num(segment_display_t *sd, uint8_t num);
void segment_display_array_show(segment_display_array_t *sd_arr,
                                uint8_t *digits, size_t size);

#endif
