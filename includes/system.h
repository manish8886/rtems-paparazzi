/*
 * system.h
 *
 *  Created on: May 29, 2013
 *      Author: manish
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define CONFIGURE_INIT



#include <rtems.h>
#include "system_time_config.h"
/* configuration information */

#include <bsp.h> /* for device driver prototypes */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT
#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TIMERS 8
#define CONFIGURE_MAXIMUM_TASKS            1
#define  CONFIGURE_MICROSECONDS_PER_TICK (1000000/SYS_TIME_FREQUENCY)
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 10
//#define CONFIGURE_USE_MINIIMFS_AS_BASE_FILESYSTEM
#include <rtems/confdefs.h>
#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GPTIMER
#define	CONFIGURE_DRIVER_AMBAPP_GAISLER_APBUART
#include <drvmgr/drvmgr_confdefs.h>


#endif /* SYSTEM_H_ */
