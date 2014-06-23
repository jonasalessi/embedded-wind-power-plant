/*
 * agente.c
 *
 *  Created on: Jun 16, 2014
 *      Author: jonas
 */
#include "agente.h"

//------ Potencia -----
uint16_t pot_nao_respondeu = 0;
unsigned char buffer_potencia[SIZE_BUFFER_POTENCIA];
//-------------------
//---- Temperatura ------
uint16_t temp_nao_respondeu = 0;
char buffer_temperatura[SIZE_BUFFER_TEMPERATURA];

//----- RPM -------
uint16_t rpm_nao_respondeu = 0;
unsigned char buffer_RPM[SIZE_BUFFER_RPM];

void enviar_temp_supervisorio() {
	uint16_t x = 0;
	for (;x < SIZE_BUFFER_TEMPERATURA;x++) {
		uart_send(buffer_temperatura[x]);
	}
}
void enviar_RPM_supervisorio() {
	uint16_t x = 0;
	for (;x < SIZE_BUFFER_RPM;x++) {
		uart_send(buffer_RPM[x]);
	}
}
void enviar_potencia_supervisorio() {
	uint16_t x = 0;
	for (;x < SIZE_BUFFER_POTENCIA;x++) {
		uart_send(buffer_potencia[x]);
	}
}
uint16_t ler_agente_potencia(float *corrente, float *tensao){
	ativa_cs(CS_POTENCIA);

	spi_transmit('I');
	///envia um lixo
	spi_transmit(0xFF);
	//iniciar captura
	buffer_potencia[0] = spi_transmit(0xFF);
	if (buffer_potencia[0] != '[') {
		return ++pot_nao_respondeu;
	}
	buffer_potencia[1] = 'P';
	buffer_potencia[2] = spi_transmit(0xFF);
	buffer_potencia[3] = spi_transmit(0xFF);
	char low1  = spi_transmit(0xFF);
	char high1 = spi_transmit(0xFF);
	if (low1 != 0x00) {
		buffer_potencia[4] = low1;
	} else {
		buffer_potencia[4] = CHAR_O;
	}
	if (high1 != 0x00) {
		buffer_potencia[5] = high1;
	} else {
		buffer_potencia[5] = CHAR_O;
	}
	char low2  = spi_transmit(0xFF);
	char high2 = spi_transmit(0xFF);
	if (low2 != 0x00) {
		buffer_potencia[6] = low2;
	} else {
		buffer_potencia[6] = CHAR_O;
	}
	if (high2 != 0x00) {
		buffer_potencia[7] = high2;
	} else {
		buffer_potencia[7] = CHAR_O;
	}
	buffer_potencia[8] = spi_transmit(0xFF);

	spi_transmit(0xFF);
	desativa_cs(CS_POTENCIA);

	uint16_t valor = low1 | (high1 << 8) ;
	float tens = valor * (5.0 / 1023.0) * 2.0;
	(*tensao) = tens;
	valor = low2 | (high2 << 8) ;

	float corrent = valor *(5.0 / 1023.0);
	corrent = corrent / 10.0;
	corrent =  (corrent / 2.15) * 1000;
	(*corrente) = corrent;

	pot_nao_respondeu = 0;

	return 0;
}

uint16_t ler_agente_temperatura(float *temperatura) {
	ativa_cs(CS_TEMPERATURA);

	uint8_t x = 0;
	spi_transmit('I');
	spi_transmit(0xFF);
	for (; x < SIZE_BUFFER_TEMPERATURA;x++) {
		buffer_temperatura[x] = spi_transmit(0xFF);
	}

	desativa_cs(CS_TEMPERATURA);

	if (buffer_temperatura[0] != '[' && buffer_temperatura[1] != 'T') {
		return ++temp_nao_respondeu;
	}

	char low  = buffer_temperatura[4];
	char high = buffer_temperatura[5];
	if (low == 0x00) {
		buffer_temperatura[4] = CHAR_O;
	}

	if (high == 0x00) {
		buffer_temperatura[5] = CHAR_O;
	}
	uint16_t valor = low | (high << 8) ;
	(*temperatura) = (valor * 100.0 * 5.0) /1023.0;

	temp_nao_respondeu = 0;

	return 0;
}


uint16_t ler_agente_RPM(uint16_t *rpm){
	unsigned char buffer_temporario[SIZE_BUFFER_RPM];
	ativa_cs(CS_RPM);
	uint8_t x = 0;
	spi_transmit('I');
	spi_transmit(0xFF);
	for (; x < SIZE_BUFFER_RPM;x++) {
		buffer_temporario[x] = spi_transmit(0xFF);
	}
	desativa_cs(CS_RPM);

	//uart_sendSTR(bufferRPM);
	//return 0;
	if (buffer_temporario[0] != '[' && buffer_temporario[1] != 'R') {
		return ++rpm_nao_respondeu;
	}
	memcpy(buffer_RPM, buffer_temporario, SIZE_BUFFER_RPM);
	uint16_t rpmTemp  = buffer_RPM[4] | (buffer_RPM[5] << 8);

	if (buffer_RPM[4] == 0x00) {
		buffer_RPM[4] = CHAR_O;
	}

	if (buffer_RPM[5] == 0x00) {
		buffer_RPM[5] = CHAR_O;
	}

	///TEMPORARIO TEMOS QUE VER PQ VEM -1, esta vindo null
	if (rpmTemp == -1 || rpmTemp > 1500) {
		return 2;
	}

	(*rpm) = rpmTemp;

	rpm_nao_respondeu = 0;

	return 0;
}
