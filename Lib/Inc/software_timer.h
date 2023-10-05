#ifndef __SOFTWARE_TIMER_H
#define __SOFTWARE_TIMER_H

#include "main.h"

#ifndef SOFTWARE_TIMERS_COUNT
#define SOFTWARE_TIMERS_COUNT 0 
#endif

typedef struct {
  size_t index;
  uint32_t interval;
} software_timer_handle_t;

extern uint32_t software_timer_counts[SOFTWARE_TIMERS_COUNT];
extern bool software_timer_flags[SOFTWARE_TIMERS_COUNT];

inline void software_timer_reset(size_t i, uint32_t count) {
  software_timer_counts[i] = count;
  software_timer_flags[i] = false;
}

inline void software_timer_item_reset(software_timer_handle_t *ti) {
  software_timer_reset(ti->index, ti->interval);
}
#define IF_SOFTWARE_TIMER_ITEM_FLAGGED(ti)                                     \
  for (; software_timer_flags[(ti)->index]; software_timer_item_reset(ti))

#endif
