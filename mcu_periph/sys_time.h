/*
 * sys_time.h
 *
 *  Created on: Apr 6, 2013
 *      Author: manish
 */
#ifndef SYS_TIME_H_
#define SYS_TIME_H_
#include <rtems.h>
#include <rtems/rtems/timer.h>
#include <types.h>
#include "../includes/types.h"
#include "../includes/std.h"

#ifndef SYS_TIME_NB_TIMER
#define SYS_TIME_NB_TIMER 8
#endif

typedef void (*sys_time_cb) (tid_t id);

//this structure is private to the timer appi and
//will not be shared to the application
struct sys_time_timer {
	int index_array;//-1 if nobody is using it
	rtems_id sys_id;
	volatile bool_t elapsed;
	rtems_interval  duration; ///< in RTEMS_TICKS
	sys_time_cb     cb;
	bool_t          in_use;
	uint32_t        end_time; ///< in SYS_TIME_TICKS
};

//extern struct sys_time_timer timers[SYS_TIME_NB_TIMER];


struct sys_time {
  volatile uint32_t nb_sec;       ///< full seconds since startup
  volatile uint32_t nb_sec_rem;   ///< remainder of seconds since startup in CPU_TICKS
  volatile uint32_t nb_tick;      ///< SYS_TIME_TICKS since startup
  struct sys_time_timer timer[SYS_TIME_NB_TIMER];

  float resolution;               ///< sys_time_timer resolution in seconds
  uint32_t ticks_per_sec;         ///< sys_time ticks per second (SYS_TIME_FREQUENCY)
  uint32_t resolution_cpu_ticks;  ///< sys_time_timer resolution in cpu ticks
  uint32_t cpu_ticks_per_sec;     ///< cpu ticks per second
};
extern struct sys_time sys_time;


extern rtems_interval ticks_per_second;




extern void sys_time_init(void);

/**
 * Register a new system timer.
 * @param duration Duration in seconds until the timer elapses.
 * @param cb Callback function that is called when timer elapses, or NULL
 * @return -1 if it failed, the timer id otherwise
 */
extern int sys_time_register_timer(float duration, sys_time_cb cb);
/**
 * Cancel a system timer by id.
 * @param id Timer id.
 */
extern void sys_time_cancel_timer(tid_t id);
extern void sys_time_update_timer(tid_t id, float duration);
extern bool_t sys_time_check_and_ack_timer(tid_t id);
static inline uint32_t msec_of_sys_time_ticks(uint32_t ticks) {
  return ticks * 1000 / sys_time.ticks_per_sec;
}
#endif /* SYS_TIME_H_ */
