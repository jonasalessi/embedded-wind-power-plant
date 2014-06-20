/*
 * agente.h
 *
 *  Created on: Jun 16, 2014
 *      Author: jonas
 */

#ifndef AGENTE_H_
#define AGENTE_H_

//tamanho dos buffer
#define SIZE_BUFFER_POTENCIA 9
#define SIZE_BUFFER_TEMPERATURA 7
#define SIZE_BUFFER_RPM 7

#include "../utils.h"
#include "../spi/spi.h"
#include "../uart/uart.h"
#include "../freio.h"
#include <string.h>

uint16_t ler_agente_potencia(float *corrente, float *tensao);
uint16_t ler_agente_temperatura(float *temperatura);
uint16_t ler_agente_RPM(uint16_t *rpm);
void enviar_temp_supervisorio();
void enviar_RPM_supervisorio();
void enviar_potencia_supervisorio();

#endif /* AGENTE_H_ */
