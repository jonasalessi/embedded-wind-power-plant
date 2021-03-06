#include <util/delay.h>
#include  <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#define DEBUG 0
#if DEBUG
	#include "uart.h"
#endif

//----------------- MACROS ---------------------------------
// ------------ (PORTx, BIT) -------------------------------
#define cpl_bit(Y, bit_x) (Y ^= (1<<bit_x))  // Mudar o estado logico de um bit
#define tst_bit(Y, bit_x) (Y & (1<<bit_x))   //Ler um bit
#define clr_bit(Y, bit_x) (Y &= ~(1<<bit_x)) //Desativar o bit (Colocar em 0)
#define set_bit(Y, bit_x) (Y |= (1<<bit_x))  //Ativar o bit (Coloca em 1)
//-----------------------------------------------------------
#define MOSI     PB3//entrada
#define MISO     PB4//saida
#define SCK      PB5//entrada
#define DDR_SPI  DDRB
#define SIZE_BUFFER 7

void spi_init_slave();
void adc_init();
uint16_t adc_read(uint8_t ch);
void init_IO();
void ler_temperatura();
void trocar_sensor();
char sensor_ativo();

char buffer[SIZE_BUFFER];
uint16_t count = 0;
uint8_t init = 0;
uint8_t sensor = 0;

int main(void) {
	init_IO();
	sei();
	_delay_ms(10);
#if DEBUG
	uart_init();
	uart_sendSTR("Iniciou Temperatura\n");
#endif
	while (1);
	return 0;
}

ISR(SPI_STC_vect) {
	cli();
	if (SPDR == 'I') {
#if DEBUG
		uart_sendSTR("I ");
#endif
		ler_temperatura();
		init = 1;
		SPDR = 0xFF;
	} else if (init){
		SPDR = (buffer[count]);
#if DEBUG
		uart_send(buffer[count]);
#endif
		count++;
		if (count == SIZE_BUFFER) {
			init = 0;
			count = 0;
#if DEBUG
			uart_sendSTR(" FIM\n");
#endif
		}
	}
	sei();
}

void trocar_sensor() {
	if (sensor == 0) {
		sensor = 1;
		return;
	}
	sensor = 0;
}

char sensor_ativo() {
	if (sensor == 1) {
		return 'B';
	}
	return 'A';
}
//Metodo para iniciar como slave
void spi_init_slave(){
	DDR_SPI = (1 << MISO);
	SPCR = (1 << SPE) | (1 << SPIE);
}

void adc_init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
uint16_t adc_read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return (ADC);
}
void init_IO() {
	clr_bit(DDRC,PC0);
	clr_bit(PORTC,PC0);
	clr_bit(DDRC,PC1);
	clr_bit(PORTC,PC1);
	spi_init_slave();
	adc_init();
}

void ler_temperatura() {
	uint16_t saida_adc = adc_read(sensor);
	//int index = 0;
	char low = (char)saida_adc;
	char high = saida_adc >> 8;
	//envia um lixo para gerar clock
	//spi_transmit(0xFF);
	buffer[0] = '['; //inicia o pacote dos dados
	buffer[1] = 'T';
	 //identificador de qual sensor está funcionando
	buffer[2] = sensor_ativo();
	buffer[3] = ',';//separador da identificação + valores
	buffer[4] = low;
	buffer[5] = high;
	buffer[6] = ']'; // finaliza o pacote de dados

	//converte para grau celsius
	uint16_t saida = (saida_adc * 100.0 * 5.0) /1023.0;
	if (saida  > 100 || saida <= 0) {
		trocar_sensor();
	}
}
