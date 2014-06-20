/*
 * spi.c
 *
 *  Created on: May 27, 2014
 *      Author: jonas
 */
#include "spi.h"

void spi_init_master(){
	DDRD =0xFF;

	//MOSI, SCK and SS são saidas(se for usar dois mestres então deve-se o SS como entrada)
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<CS_POTENCIA) |(1<<CS_TEMPERATURA) | (1 << CS_RPM);

	//MISO é entrada
	DDRB &= (~(1<<MISO));

	SPCR = ((1<<SPE)|(1<<MSTR)|(1<SPR0));

	//Desativa os CS
	PORTB |= (1<< CS_POTENCIA) | (1 << CS_TEMPERATURA) | (1 << CS_RPM);
}

char spi_transmit(char dado) {
	SPDR = dado;
	while(!(SPSR & (1 << SPIF)));
	_delay_ms(1);
	return SPDR;
}
