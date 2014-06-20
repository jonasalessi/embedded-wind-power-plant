#include "uart.h"

// Define baud rate
//#define USART_BAUDRATE 38400
#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void uart_init(void) {
	UBRR0L = BAUD_PRESCALE;
	UBRR0H = (BAUD_PRESCALE >> 8);
	UCSR0B = ((1<<TXEN0)|(1<<RXEN0) | (1<<RXCIE0));
}

void uart_send(unsigned char c) {
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}
void uart_sendSTR(char *str) {
	while (*str)
	uart_send(*str++);
}
