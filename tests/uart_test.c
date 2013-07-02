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

rtems_task Init(
  rtems_task_argument argument
);
#define wc 5
static const char* name[wc]={"Msg1","Msg2","Msg3","Msg4","Msg5"};
static bool bstop = false;
void timer_cb(tid_t tid){
	bstop = true;
}

rtems_task Init(
  rtems_task_argument ignored
){
	PeriodicSendAp(UART,1);
	sys_time_init();
	UART1Init();

	sys_time_register_timer(5*60,timer_cb);
	int cnt = 0;
	while(!bstop){
		int i =0;
		while(name[cnt][i]!=0){
			UART1Transmit(name[cnt][i]);
			i++;
		}
		UART1Transmit(name[cnt][i]);
		UART1Transmit('\n');
		cnt++;
		cnt = (cnt)%wc;
	}
	UART1Transmit('Z');
	rtems_task_wake_after(1000);
	exit(0);
}
























