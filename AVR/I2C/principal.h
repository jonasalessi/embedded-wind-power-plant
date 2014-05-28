/*
 * principal.h
 *
 * Created: 05/01/2014 22:04:58
 *  Author: Jonas Alessi
 */ 


#ifndef PRINCIPAL_H_
#define PRINCIPAL_H_


#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

/*------------------------------------------------------
						MACROS
------------ (PORTx,BIT) -------------------------------*/
#define cpl_bit(Y,bit_x) (Y^=(1<<bit_x)) // Mudar o estado logico de um bit
#define tst_bit(Y,bit_x) (Y&(1<<bit_x))  //Ler um bit
#define clr_bit(Y,bit_x) (Y&=~(1<<bit_x)) //Desativar o bit (Colocar em 0)
#define set_bit(Y,bit_x) (Y|=(1<<bit_x)) //Ativar o bit (Coloca em 1)
/*----------
 END MACROS
 -----------*/

#define OK PB1
#define ERROR PB0
#define onErro() set_bit(PORTB,ERROR)
#define offErro() clr_bit(PORTB,ERROR)
#define onOK() set_bit(PORTB,OK)
#define offOk() clr_bit(PORTB,OK)
#define true 0x01
#define false 0x00

typedef unsigned char boolean;

#endif /* PRINCIPAL_H_ */