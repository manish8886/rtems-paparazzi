/*
 * uart.h
 *
 *  Created on: May 24, 2013
 *      Author: manish
 */

#ifndef UART_H_
#define UART_H_
#include "../includes/types.h"
#ifdef USE_UART0
//#define Uart0InitParam(_b, _m, _fm) uart_periph_init_param(&uart0, _b, _m, _fm, "")
void UART0Init(void);
void UART0CheckFreeSpace(uint8_t _x);
void UART0Transmit(uint8_t _x);
inline void UART0SendMessage(){
	return;
}
bool UART0ChAvailable(void);
uint8_t UART0Getch(void);
uint8_t UART0TxRunning(void);
//consider hardware flow when making actual call for changing the
//baud rate.
void UART0SetBaudrate(uint32_t baudrate);
#endif // USE_UART0

#ifdef USE_UART1
void UART1Init(void);
void UART1CheckFreeSpace(uint8_t _x);
void UART1Transmit(uint8_t _x);
inline void UART1SendMessage(){
	return;
}
bool UART1ChAvailable(void);
uint8_t UART1Getch(void);
uint8_t UART1TxRunning(void);
//consider hardware flow when making actual call for changing the
//baud rate.
void UART1SetBaudrate(uint32_t baudrate);
#endif
//#define Uart1InitParam(_b, _m, _fm) uart_periph_init_param(&uart1, _b, _m, _fm, "")


#ifdef USE_UART2
void UART2Init(void);
void UART2CheckFreeSpace(uint8_t _x);
void UART2Transmit(uint8_t _x);
inline void UART2SendMessage(){
	return;
}
bool UART2ChAvailable(void);
uint8_t UART2Getch(void);
uint8_t UART2TxRunning(void);
//consider hardware flow when making actual call for changing the
//baud rate.
void UART2SetBaudrate(uint32_t baudrate);
#endif // USE_UART2

#ifdef USE_UART3
void UART3Init(void);
void UART3CheckFreeSpace(uint8_t _x);
void UART3Transmit(uint8_t _x);
inline void UART3SendMessage(){
	return;
}
bool UART3ChAvailable(void);
uint8_t UART3Getch(void);
uint8_t UART3TxRunning(void);
//consider hardware flow when making actual call for changing the
//baud rate.
void UART3SetBaudrate(uint32_t baudrate);
#endif // USE_UART3

#ifdef USE_UART5
void UART5Init(void);
void UART5CheckFreeSpace(uint8_t _x);
void UART5Transmit(uint8_t _x);
inline void UART5SendMessage(){
	return;
}
bool UART5ChAvailable(void);
uint8_t UART5Getch(void);
uint8_t UART5TxRunning(void);
//consider hardware flow when making actual call for changing the
//baud rate.
void UART5SetBaudrate(uint32_t baudrate);

#endif // USE_UART5

#endif /* UART_H_ */
