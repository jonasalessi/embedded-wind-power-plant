/*
 * utils.h
 *
 *  Created on: Jun 5, 2014
 *      Author: jonas
 */

#ifndef UTILS_H_
#define UTILS_H_

#include  <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

//----------------- MACROS ---------------------------------
// ------------ (PORTx, BIT) -------------------------------
#define cpl_bit(Y, bit_x) (Y ^= (1<<bit_x))  // Mudar o estado logico de um bit
#define tst_bit(Y, bit_x) (Y & (1<<bit_x))   //Ler um bit
#define clr_bit(Y, bit_x) (Y &= ~(1<<bit_x)) //Desativar o bit (Colocar em 0)
#define set_bit(Y, bit_x) (Y |= (1<<bit_x))  //Ativar o bit (Coloca em 1)
#define ativa_cs(bit_X) clr_bit(PORTB,bit_X)
#define desativa_cs(bit_X) set_bit(PORTB,bit_X)

//caracter para controle no supervisorio
#define CHAR_O '#'
//temperatura
#define TEMP_MAXIMA 30  //verificar
#define TEMP_MINIMA 20  //verificar
//RPM
#define RPM_MAXIMO  800  //verificar
#define RPM_MINIMO  600  //verificar
///freio
#define FREIO_MAX 2300
#define FREIO_MIN 999
//alerta
#define ALERT_ON PORTD |= (1 << PD7)
#define ALERT_OFF PORTD &= ~(1 << PD7)
// DEBUG
#define DEBUG 0

//-----------------------------------------------------------

#endif /* UTILS_H_ */
