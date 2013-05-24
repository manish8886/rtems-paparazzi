/*
 * uart.c
 *
 *  Created on: May 24, 2013
 *      Author: Manish
 */

#include "uart.h"
typedef static struct{
	const char* uart_name; //name of the uart device.
	int fd;//file descriptor.
}uart_dev_info;

uart_dev_info uart_dev_arry[5]={
										{"/dev/apbuart0",-1 },
										{"/dev/apbuart1",-1 },
										{"/dev/apbuart2",-1 },
										{"/dev/rastaio0/apbuart0",-1 },
										{"/dev/rastaio0/apbuart1",-1 }
									};

static void uart_init(uart_dev_info* dev_info);
static bool uart_checkfreespace(const uart_dev_info* dev_info);
static void uart_transmit(const uart_dev_info* dev_info,uint8_t data);
static bool uart_chavailable(const uart_dev_info* dev_info);
static uint8_t uart_getch(const uart_dev_info* dev_info);
static bool uart_txrunning(const uart_dev_info* dev_info);
static uint8_t uart_setbaudrate(const uart_dev_info* dev_info, uint32_t baudrate,
		bool_t hw_Ctrl_flow);
