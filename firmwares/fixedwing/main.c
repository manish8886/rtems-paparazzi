/*
 * Copyright (C) 2005 Pascal Brisset, Antoine Drouin
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file firmwares/fixedwing/main.c
 * Main loop used both on single and dual MCU configuration.
 */

#include <amba.h>
#include <drvmgr/ambapp_bus.h>
#include <system.h>
#include <rtems/termiostypes.h>
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

#ifdef FBW
#include "firmwares/fixedwing/main_fbw.h"
#define Fbw(f) f ## _fbw()
#else
#define Fbw(f)
#endif

#ifdef AP
#include "firmwares/fixedwing/main_ap.h"
#define Ap(f) f ## _ap()
#else
#define Ap(f)
#endif
rtems_task Init(
  rtems_task_argument ignored
){
  Fbw(init);
  Ap(init);
  while (1) {
    Fbw(handle_periodic_tasks);
    Ap(handle_periodic_tasks);
    Fbw(event_task);
    Ap(event_task);
  }
  return 0;
}
