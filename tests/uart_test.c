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


rtems_task Init(
  rtems_task_argument ignored
)
{
	int i = 0;
	int newcnt =0 ;
	UART1Init();
	while(i<128){
		if('a'+(i) <='z'){
			UART1Transmit('a'+(i));
		}else if(newcnt <= 9 ){
			UART1Transmit('1'+newcnt);
			newcnt++;
		}
		if(newcnt >= 10) newcnt =0 ;
		i++;
	}


	rtems_task_wake_after(1000);
	exit(0);
}
























