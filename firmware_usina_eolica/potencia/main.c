#define F_CPU 8000000UL // clock

#include <util/delay.h>
#include  <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
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

void spi_init_slave();
void adc_init();
uint16_t adc_read(uint8_t ch);
void init_IO();
void ler_tensao_corrente();

unsigned char buffer[8];
uint16_t count = 0;
uint8_t init = 0;

int main(void) {
	init_IO();
	sei();
	_delay_ms(10);
#if DEBUG
	uart_init();
	uart_sendSTR("Iniciando Agente\n");
#endif
	while (1);
	return 0;
}

ISR(SPI_STC_vect) {
	if (SPDR == 'I') {
#if DEBUG
		uart_sendSTR("I ");
#endif
		ler_tensao_corrente();
		init = 1;
		SPDR = 0xFF;
	} else if (init){
		SPDR = (buffer[count]);
#if DEBUG
		uart_send(buffer[count]);
#endif
		count++;
		if (count == 8) {
			init = 0;
			count = 0;
#if DEBUG
			uart_sendSTR(" FIM\n");
#endif
		}
	}
}

//Metodo para iniciar como slave
void spi_init_slave(){
	DDR_SPI = (1 << MISO);
	SPCR = (1 << SPE) | (1 << SPIE);
}

//configurando módulo ADC
void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
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
	spi_init_slave();
	adc_init();
}

void ler_tensao_corrente() {
	uint16_t saida_adc = adc_read(0);
	//int index = 0;
	unsigned char low0 = (unsigned char)saida_adc;
	unsigned char high0 = saida_adc >> 8;

	saida_adc = adc_read(1);
	//int index = 0;
	unsigned char low1 = (unsigned char)saida_adc;
	unsigned char high1 = saida_adc >> 8;

	buffer[0] = ('['); //inicia o pacote dos dados
	buffer[1] = ('A'); //identificador de qual sensor está funcionando
	buffer[2] = (',');//separador da identificação + valores
	buffer[3] = (low0);
	buffer[4] = (high0);
	buffer[5] = (low1);
	buffer[6] = (high1);
	buffer[7] = (']'); // finaliza o pacote de dados
}
