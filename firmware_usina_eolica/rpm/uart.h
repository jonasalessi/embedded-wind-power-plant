/*
 * uart.h
 *
 *  Created on: May 23, 2014
 *      Author: jonas
 */

#ifndef UART_H_
#define UART_H_

void uart_init(void);
void uart_send(unsigned char c);
void uart_sendSTR(char *str);


#endif /* UART_H_ */
