/*
 * system_time_config.h
 *
 *  Created on: Aug 15, 2013
 *      Author: manish
 */

#ifndef SYSTEM_TIME_CONFIG_H_
#define SYSTEM_TIME_CONFIG_H_

#ifndef SYS_TIME_FREQUENCY
#if defined PERIODIC_FREQUENCY
#define SYS_TIME_FREQUENCY (2 * PERIODIC_FREQUENCY)
#else
#define SYS_TIME_FREQUENCY 1000
#endif
#endif


#endif /* SYSTEM_TIME_CONFIG_H_ */
