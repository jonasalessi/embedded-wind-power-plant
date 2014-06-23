#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

///1 ativa DEBUG na serial. 0 desativa DEBUG na serial
#define DEBUG 0
#if DEBUG
	#include "uart.h"
#endif

#define	set_bit(Y,bit_x) (Y|=(1<<bit_x))	//ativa o bit
#define	clr_bit(Y,bit_x) (Y&=~(1<<bit_x))	//limpa o bit
#define tst_bit(Y,bit_x) (Y&(1<<bit_x))  	//testa o bit
#define cpl_bit(Y,bit_x) (Y^=(1<<bit_x))	//troca o estado do bit

#define SENSOR1 PD2			//A
#define SENSOR2 PD3			//B
#define MOSI     PB3		//entrada
#define MISO     PB4		//saida
#define SCK      PB5		//entrada
#define DDR_SPI  DDRB

unsigned long counter = 0;
unsigned long milisegundos = 0;
uint16_t rpm = 0;
uint16_t contagem_zero = 0;
char sensor_ativo = 'A';

//controle do SPI slave
unsigned char buffer[7];
uint16_t count = 0;
uint8_t init = 0;

void calcular_RPM();
void spi_init_slave();
void ativar_sensorB();
void ativar_sensorA();
void zerar();

int main() {
	spi_init_slave();

	//Ativa os dois sensores
	clr_bit(DDRD, SENSOR1);
	clr_bit(DDRD, SENSOR2);

	EICRA = (1 << ISC01) |(1 << ISC11); //interrupcoes no INT0 na borda de descida
	EIMSK = (1 << INT0); //habilita a interrupcao INT0
	TIMSK1 |= 1<<OCIE1A;
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	/*cada um segundo 7812
	 * 1000ms  7812
	 *   1 ms    x   = 8
	 */
	OCR1A= 8;

#if DEBUG
	uart_init();
	uart_sendSTR("Iniciou agente RPM\n");
#endif
	sei();
	while (1);
}

void ativar_sensorA() {
	clr_bit(EIMSK, INT1);
	set_bit(EIMSK, INT0);
	zerar();
	sensor_ativo = 'A';
}

void ativar_sensorB() {
	set_bit(EIMSK, INT1);
	clr_bit(EIMSK, INT0);
	zerar();
	sensor_ativo = 'B';
}

void zerar() {
	milisegundos = 0;
	contagem_zero = 0;
}

ISR(INT0_vect){
	calcular_RPM();
}

ISR(INT1_vect){
	calcular_RPM();
}

ISR( TIMER1_COMPA_vect) {
	milisegundos++;
	if (milisegundos == 1000) {
		milisegundos = 0;
		rpm = 0;
	}
}

ISR(SPI_STC_vect) {
	if (SPDR == 'I') {
		buffer[0] = '[';
		buffer[1] = 'R';
		buffer[2] = sensor_ativo;
		buffer[3] = ',';
		buffer[4] = (unsigned char) rpm;
		buffer[5] = (unsigned char)(rpm >> 8);
		buffer[6] = ']';
		init = 1;
		if (rpm < 0 || rpm > 1500) {
			if (sensor_ativo == 'A') {
				ativar_sensorB();
			} else {
				ativar_sensorA();
			}
		}

#if DEBUG
		char out[sizeof(uint16_t)];
		sprintf(out, "%d", rpm);
		uart_sendSTR("Sensor "); uart_send(sensor_ativo);
		uart_sendSTR(" RPM "); uart_sendSTR(out);
		uart_sendSTR(" I ");
#endif

	} else if (init) {
		SPDR = (buffer[count]);

#if DEBUG
		uart_send(buffer[count]);
#endif

		count++;
		if (count == 7) {
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

void calcular_RPM() {
	//se for zero vair ser -1 o rpm, pode ser 0 pq fica travado na interrupcao de counter
	if (milisegundos == 0) {
		rpm = 0;
	} else {
		rpm = 60000 / milisegundos;
		milisegundos = 0;
	}
}
