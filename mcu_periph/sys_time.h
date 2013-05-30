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
};
extern struct sys_time_timer timers[SYS_TIME_NB_TIMER];
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
#endif /* SYS_TIME_H_ */
