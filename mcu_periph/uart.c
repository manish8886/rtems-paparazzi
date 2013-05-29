/*
 * uart.c
 *
 *  Created on: May 24, 2013
 *      Author: Manish
 */
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <rtems.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <apbuart.h>
#include "uart.h"
/*
 * All communication to UART device will be done through the
 * file system call of RTEMS. Therefore we will need to enable
 * posix api while building the RTEMS.
 */

typedef struct {
	const char* uart_name; //name of the uart device.
	const char sem_name[4];//semaphore name
	rtems_id semaphore_id; //id for the semaphore.
	int fd;                //file descriptor for uart.
}uart_dev_info_t;

// RTEMS has following names for the UART devices.
// Initially all UART devices has "-1" as their file
// descriptor which will be considered as "Invalid Descriptor".
static uart_dev_info_t uart_dev_arry[5]={
								 {"/dev/apbuart0",{'s','e','m','0'},-1,0 },
								 {"/dev/apbuart1",{'s','e','m','1'},-1,0 },
								 {"/dev/apbuart2",{'s','e','m','2'},-1,0 },
								 {"/dev/rastaio0/apbuart0",{'s','e','m','3'},-1,0 },
								 {"/dev/rastaio0/apbuart1",{'s','e','m','4'},-1,0 }
							    };
/** Note:
 * While implementing each device interface to make it available to paparazzi I
 * needed to make a few assumptions. And for now these assumptions are valid according
 * to my current understanding. Therefore, modification in the code without considering
 * the assumptions might break something so please try to read them first before making
 * changes in the code. And if you also need to change the existing one or need to make
 * new assumptions please modify and add them to Assumption list in relevant source file.
 */
/*ASSUMPTIONS:
 * 1.All UART devices can be initialized once means once they are opened, they will have a
 * valid file discriptor and this discriptor will not be changed when some other task or same
 * task will try to initalize the opened UART again.
 * 2.We have assumed all calls like transmitting or receivng the data are non-blocking.
 * 3.If more than one task want to use the same UART at the same time access to device will
 * be syncrionised.
 */
#define TX_BUFFER_LEN					 128
#define RX_BUFFER_LEN					 128
#define MAX_UART_DEV_CNT                 5
#define ISUARTOPENED(dev_index)          (uart_dev_arry[dev_index].fd != -1) ? TRUE : FALSE
#define ISVALIDUARTINDEX(dev_index)      (((dev_index >=0) && (dev_index < MAX_UART_DEV_CNT)) ? TRUE : FALSE)

static void uart_init(int dev_index);
static bool_t uart_checkfreespace(int dev_index);
static void uart_transmit(int dev_index,uint8_t data);
static bool_t uart_chavailable(int dev_index);
static uint8_t uart_getch(int dev_index);
static bool_t uart_txrunning(int dev_index);
static uint8_t uart_setbaudrate(int dev_index,uint32_t baudrate,bool_t hw_Ctrl_flow);

/**
 * Only open the device if it is not opened already.We need to
 * set the length of the receiver and transmission buffer before
 * starting the UART.
 */
static void uart_init(int dev_index){
	//Do u want to check dev_index range also
	int fd,result;
	rtems_status_code status;
	rtems_name semaphore_name;
	rtems_id sem_id;
	if(ISUARTOPENED(dev_index))
		return;
	}
	/*Just open the UART here. Simply opening will not start the hardware.*/
	fd = open(uart_dev_arry[dev_index].uart_name,O_RDWR);
	if(fd < 0){
		fd=0;
	}
	//Now first set the length of receiver and transmission buffer.
	result = ioctl(fd,APBUART_SET_TXFIFO_LEN,(void *)TX_BUFFER_LEN);
	if(result){
	#ifdef _DEBUG
		printf("Failed to set the length of transmission Buffer for uart%d",dev_index);
	#endif
		close(fd);
		return;
	}

	result = ioctl(fd,APBUART_SET_RXFIFO_LEN,(void *)RX_BUFFER_LEN);
	if(result){
	#ifdef _DEBUG
		printf("Failed to set the length of receiver buffer for uart%d",dev_index);
	#endif
		close(fd);
		return;
	}


	//create the binary semaphore for syncronization.
	 semaphore_name = rtems_build_name( uart_dev_arry[dev_index].uart_name[0],
										uart_dev_arry[dev_index].uart_name[1],
										uart_dev_arry[dev_index].uart_name[2],
										uart_dev_arry[dev_index].uart_name[3]);
	 /*
	  * Semaphore Options:
	  * FIFO
	  * BINARY Semaphore
	  * No_Inherit_Priority
	  * No_Priority_Ceiling
	  * Local
	  */
	status = rtems_semaphore_create(semaphore_name,0,RTEMS_BINARY_SEMAPHORE,0,&sem_id);
	if(status != RTEMS_SUCCESSFUL){
	#ifdef _DEBUG
		printf("error in creating semaphore for uart%d",dev_index);
	#endif
		close(fd);
		return;
	}

	/*Just clear all the stats before starting the uart.*/
	result = ioctl(fd,APBUART_CLR_STATS);
	if(result){
	#ifdef _DEBUG
		printf("couldn't to clear the stats for uart%d",dev_index);
	#endif
	}

	/*set UART in Non-blocking mode. Because paparazzi uses the UART
	 * always as non locking.Refer the code uart_arch.c for any architecutre
	 * in Paparazzi airborne code. */

	/*Just clear all the stats before starting the uart.*/
	result = ioctl(fd,APBUART_SET_BLOCKING,0);
	if(result){
	#ifdef _DEBUG
		printf("couldn't set uart%d for non-blocking mode",dev_index);
	#endif
	}

	/*Now before returning just start the hardware uart.*/
	result = ioctl(fd,APBUART_START);
	if(result){
	#ifdef _DEBUG
		printf("Failed to start uart%d",dev_index);
	#endif
	close(fd);
	}

	uart_dev_arry[dev_index].fd = fd;
	uart_dev_arry[dev_index].semaphore_id = sem_id;
	return;
}
static bool_t uart_checkfreespace(int dev_index){

}
static void uart_transmit(int dev_index,uint8_t data){
	int result;
	int fd;
	if(!ISUARTOPENED(dev_index))
		return;
	}
	fd = uart_dev_arry
	write()

}
