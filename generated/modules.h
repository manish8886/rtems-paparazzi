/* This file has been generated from /home/manish/paprazzi-git/paparazzi/conf/airframes/examples/microjet.xml */
/* Please DO NOT EDIT */

#ifndef MODULES_H
#define MODULES_H

#define MODULES_IDLE  0
#define MODULES_RUN   1
#define MODULES_START 2
#define MODULES_STOP  3

#ifdef MODULES_C
#define EXTERN_MODULES
#else
#define EXTERN_MODULES extern
#endif
#include "std.h"
#include "openlog/openlog.h"
/*#include "sensors/infrared_adc.h"*/
//#include "digital_cam/servo_cam_ctrl.h"

#define PERIODIC_2HZ_OPENLOG_PERIOD 0.500000
#define PERIODIC_2HZ_OPENLOG_FREQ 2.000000
#define INFRARED_ADC_UPDATE_PERIOD 0.016667
#define INFRARED_ADC_UPDATE_FREQ 60.000000
#define SERVO_CAM_CTRL_PERIODIC_PERIOD 0.250000
#define SERVO_CAM_CTRL_PERIODIC_FREQ 4.000000

static uint8_t openlog_periodic_2Hz_openlog_status=0;
static uint8_t digital_cam_servo_servo_cam_ctrl_periodic_status=0;


#endif // MODULES_H
