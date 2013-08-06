/*
 * uart_test.c
 *
 *  Created on: May 29, 2013
 *      Author: manish
 */

#include <amba.h>
#include <drvmgr/ambapp_bus.h>
#include <system.h>
#include <rtems/termiostypes.h>
#include "../mcu_periph/uart.h"
#include "../mcu_periph/sys_time.h"
#include "../generated/periodic_telemetry.h"
#include "../firmwares/fixedwing/ap_downlink.h"


#include <math.h>
#include "mcu_periph/sys_time.h"

// Sensors
#if USE_GPS
#include "subsystems/gps.h"
#endif
#if USE_IMU
#include "subsystems/imu.h"
#endif
#if USE_AHRS
#include "subsystems/ahrs.h"
#endif
#if USE_AHRS_ALIGNER
#include "subsystems/ahrs/ahrs_aligner.h"
#endif
#if USE_BAROMETER
#include "subsystems/sensors/baro.h"
#endif
#include "subsystems/ins.h"


// autopilot & control
#include "state.h"
#include "firmwares/fixedwing/autopilot.h"
#include "firmwares/fixedwing/stabilization/stabilization_attitude.h"
#include CTRL_TYPE_H
#include "subsystems/nav.h"
#include "generated/flight_plan.h"
#ifdef TRAFFIC_INFO
#include "subsystems/navigation/traffic_info.h"
#endif

// datalink & telemetry
#include "subsystems/datalink/datalink.h"
#include "subsystems/settings.h"
#include "firmwares/fixedwing/ap_downlink.h"

// modules & settings
#include "generated/modules.h"
#include "generated/settings.h"
#if defined RADIO_CONTROL || defined RADIO_CONTROL_AUTO1
#include "rc_settings.h"
#endif



/*set the uart resource as input driven and set this resource when uart get initialised.*/
struct drvmgr_key grlib_drv_res_apbuart0[] ={
												{"mode",KEY_TYPE_INT,((unsigned int)TERMIOS_IRQ_DRIVEN)},
												{"syscon",KEY_TYPE_INT,((unsigned int)0)},
												{"dbgcon", KEY_TYPE_INT, ((unsigned int)0)},
												KEY_EMPTY
											};


struct drvmgr_key grlib_drv_res_apbuart1[] ={
												{"mode",KEY_TYPE_INT,((unsigned int)TERMIOS_IRQ_DRIVEN)},
												{"syscon",KEY_TYPE_INT,((unsigned int)0)},
												{"dbgcon", KEY_TYPE_INT, ((unsigned int)0)},
												KEY_EMPTY
											};





/*it will override the grlib_drv_resources in the amba.c*/
struct drvmgr_bus_res grlib_drv_resources  ={
												.next = NULL,
												.resource = {
																{DRIVER_AMBAPP_GAISLER_APBUART_ID, 0,&grlib_drv_res_apbuart0[0]},
																//{DRIVER_AMBAPP_GAISLER_APBUART_ID, 1,&grlib_drv_res_apbuart1[0]},
																RES_EMPTY
															}
											};

//declare static variable here
static uint8_t  mcu1_status;
static int32_t current;

rtems_task Init(
  rtems_task_argument argument
);
#define wc 5
//static const char* name[wc]={"Msg1","Msg2","Msg3","Msg4","Msg5"};
static bool bstop = false;
void timer_cb(tid_t tid){
	bstop = true;
}

extern void sim_use_gps_pos(double lat, double lon, double alt, double course, double gspeed, double climb, double time);
extern void sim_update_sv();

rtems_task Init(
  rtems_task_argument ignored
){
	sys_time_init();
	UART1Init();
	sys_time_register_timer(5*60,timer_cb);
	unsigned int cnt = 0,i=0;
	sim_use_gps_pos(1.0, 2.0, 3.0, 4.0, 5.0,6.0,7.0);
	sim_update_sv();
	while(!bstop){
		//UART1Transmit('a'+ ((i++)%26));
		PeriodicSendAp(DefaultChannel, DefaultDevice);
#if USE_IVY
		PeriodicSendAp(IVYCHANNEL, IVYDEVICE);
#endif
	}
	//UART1Transmit('Z');
	rtems_task_wake_after(1000);
	exit(0);
}
























