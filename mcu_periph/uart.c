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
static bool_t uart_checkfreespace(int dev_index,uint8_t len);
static void uart_transmit(int dev_index,uint8_t data);
static bool_t uart_chavailable(int dev_index);
static uint8_t uart_getch(int dev_index);
static bool_t uart_txrunning(int dev_index);
static void uart_setbaudrate(int dev_index,uint32_t baudrate,bool_t hw_Ctrl_flow);

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
	if(ISUARTOPENED(dev_index)){
		return;
	}
	/*Just opening will not start the UART hardware.*/
	fd = open(uart_dev_arry[dev_index].uart_name,O_RDWR);
	if(fd < 0){
		fd=0;
	}
	//Now first set the length of receiver and transmission buffer.
	result = ioctl(fd,APBUART_SET_TXFIFO_LEN,(void *)TX_BUFFER_LEN);
	if(result < 0){
	#ifdef _DEBUG
		printf("Failed to set the length of transmission Buffer for uart%d",dev_index);
	#endif
		close(fd);
		return;
	}

	result = ioctl(fd,APBUART_SET_RXFIFO_LEN,(void *)RX_BUFFER_LEN);
	if(result < 0){
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
	if(result < 0){
	#ifdef _DEBUG
		printf("couldn't to clear the stats for uart%d",dev_index);
	#endif
	}

	/*set UART in Non-blocking mode. Because paparazzi uses the UART
	 * always as non locking.Refer the code uart_arch.c for any architecutre
	 * in Paparazzi airborne code. */

	/*Just clear all the stats before starting the uart.*/
	result = ioctl(fd,APBUART_SET_BLOCKING,0);
	if(result < 0){
	#ifdef _DEBUG
		printf("couldn't set uart%d for non-blocking mode",dev_index);
	#endif
	}

	/*Now before returning just start the hardware uart.*/
	result = ioctl(fd,APBUART_START);
	if(result < 0){
	#ifdef _DEBUG
		printf("Failed to start uart%d",dev_index);
	#endif
	close(fd);
	}

	uart_dev_arry[dev_index].fd = fd;
	uart_dev_arry[dev_index].semaphore_id = sem_id;
	return;
}

static void uart_transmit(int dev_index,uint8_t data){
	int fd;
	size_t bytes;
	if(!ISUARTOPENED(dev_index)){
		return;
	}
	fd = uart_dev_arry[dev_index].fd;
	bytes = write(fd,&data,1);
	if(bytes < 0){
	#ifdef _DEBUG
		printf("Error in writing data to UART %d",dev_index);
	#endif
	}
	return;
}
static uint8_t uart_getch(int dev_index){
	int fd;
	uint8_t data=0;
	size_t bytes;

	if(!ISUARTOPENED(dev_index)){
		return 0;
	}
	fd = uart_dev_arry[dev_index].fd;
	bytes = read(fd,&data,1);
	if(bytes < 0){
	#ifdef _DEBUG
		printf("Error in reading data to UART %d",dev_index);
	#endif
	}

	return data;
}
static void uart_setbaudrate(int dev_index,uint32_t baudrate,bool_t hw_Ctrl_flow){
	int fd;
	int result;
	if(!ISUARTOPENED(dev_index)){
			return;
	}
	fd = uart_dev_arry[dev_index].fd;
	//for now ignoring the hw_Ctrl_flow parameter.
	(void)hw_Ctrl_flow;
	result = ioctl(fd,APBUART_SET_BAUDRATE,baudrate);
	if(result < 0){
	#ifdef _DEBUG
		printf("couldn't set the baud rate of UART %d",dev_index);
	#endif
	}
	return ;
}
static bool_t uart_txrunning(int dev_index){
	bool_t result=true;
	(void)dev_index;
	(void)result;
	return result;
}
static bool_t uart_checkfreespace(int dev_index,uint8_t len){
	bool_t result=true;
	(void)dev_index;
	(void)len;
	(void)result;
	return result;

}
static bool_t uart_chavailable(int dev_index){
	bool_t result=true;
	(void)dev_index;
	(void)result;
	return result;
}
#ifdef USE_UART0
void UART0Init(void){
	uart_init(0);
}
bool_t UART0CheckFreeSpace(uint8_t len){
	return uart_checkfreespace(0,len);
}

void UART0Transmit(uint8_t data){
	return uart_transmit(0,data);
}
bool_t UART0ChAvailable(void){
	return uart_chavailable(0);
}
uint8_t UART0Getch(void){
	return uart_getch(0);
}
bool_t UART0TxRunning(void){
	return uart_txrunning(0);
}

void UART0SetBaudrate(uint32_t baudrate){
	return uart_setbaudrate(0,baudrate,false);
}
#endif

#ifdef USE_UART1
void UART1Init(void){
	uart_init(0);
}
bool_t UART1CheckFreeSpace(uint8_t len){
	return uart_checkfreespace(0,len);
}

void UART1Transmit(uint8_t data){
	return uart_transmit(0,data);
}
bool_t UART0ChAvailable(void){
	return uart_chavailable(0);
}
uint8_t UART1Getch(void){
	return uart_getch(0);
}
bool_t UART1TxRunning(void){
	return uart_txrunning(0);
}

void UART1SetBaudrate(uint32_t baudrate){
	return uart_setbaudrate(0,baudrate,false);
}
#endif
#ifdef USE_UART2
void UART2Init(void){
	uart_init(0);
}
bool_t UART2CheckFreeSpace(uint8_t len){
	return uart_checkfreespace(0,len);
}

void UART2Transmit(uint8_t data){
	return uart_transmit(0,data);
}
bool_t UART2ChAvailable(void){
	return uart_chavailable(0);
}
uint8_t UART2Getch(void){
	return uart_getch(0);
}
bool_t UART2TxRunning(void){
	return uart_txrunning(0);
}
void UART2SetBaudrate(uint32_t baudrate){
	return uart_setbaudrate(0,baudrate,false);
}
#endif
#ifdef USE_UART3
void UART3Init(void){
	uart_init(0);
}
bool_t UART3CheckFreeSpace(uint8_t len){
	return uart_checkfreespace(0,len);
}

void UART3Transmit(uint8_t data){
	return uart_transmit(0,data);
}
bool_t UART3ChAvailable(void){
	return uart_chavailable(0);
}
uint8_t UART3Getch(void){
	return uart_getch(0);
}
bool_t UART3TxRunning(void){
	return uart_txrunning(0);
}
void UART3SetBaudrate(uint32_t baudrate){
	return uart_setbaudrate(0,baudrate,false);
}
#endif
#ifdef USE_UART5
void UART5Init(void){
	uart_init(0);
}
bool_t UART5CheckFreeSpace(uint8_t len){
	return uart_checkfreespace(0,len);
}

void UART5Transmit(uint8_t data){
	return uart_transmit(0,data);
}
bool_t UART5ChAvailable(void){
	return uart_chavailable(0);
}
uint8_t UART5Getch(void){
	return uart_getch(0);
}
bool_t UART5TxRunning(void){
	return uart_txrunning(0);
}

void UART5SetBaudrate(uint32_t baudrate){
	return uart_setbaudrate(0,baudrate,false);
}
#endif

