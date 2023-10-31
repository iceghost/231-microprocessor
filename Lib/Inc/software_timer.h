#ifndef __SOFTWARE_TIMER_H
#define __SOFTWARE_TIMER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern uint32_t software_timer_tick;

typedef struct {
  uint32_t start;
  uint32_t interval;
  bool flag;
} software_timer_t;

inline void software_timer_init(software_timer_t *st, uint32_t interval) {
  st->interval = interval;
  st->start = software_timer_tick;
  st->flag = false;
}

void software_timers_start(software_timer_t *sts, size_t count);

inline void software_timer_reset(software_timer_t *st) {
  st->start = software_timer_tick;
  st->flag = false;
}

#define software_timer_if_flagged(st)                                          \
  for (; (st)->flag; software_timer_reset(st))

#endif
