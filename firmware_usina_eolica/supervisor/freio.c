/*
 * freio.c
 *
 *  Created on: Jun 16, 2014
 *      Author: jonas
 */
#include "freio.h"

void inicializarFreio() {
	//habilita o OC1A
	set_bit(DDRB,PB1);
	clr_bit(PORTB,PB1);

	ICR1 = 39999; //valor para a maxima contagem

	// configurar o TC1 para o modo PWM rapido via ICR1, preescaler = 8
	set_bit(TCCR1A,WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	set_bit(TCCR1A, COM1A1);
}

/**
 * Servo. Grau = largura do pulso
 * 0  = 0,5ms  -> 999
 * 45 = 1.0ms  -> 1999
 * 90 = 1.5ms  -> 2999
 * 135= 2.0ms  -> 3999
 * 180= 2.5ms  -> 4999
 */
void frear() {
	OCR1A = FREIO_MAX;
}

void desfreiar(){
	OCR1A=FREIO_MIN;
}

