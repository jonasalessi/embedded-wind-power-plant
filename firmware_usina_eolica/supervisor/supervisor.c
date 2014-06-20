/*
 * SUPERVISOR
 */
#define F_CPU 8000000UL

#include "utils.h"
#include "uart/uart.h"
#include "spi/spi.h"
#include "agente/agente.h"

uint8_t _rpm_nao_respondeu = 0;
uint8_t _temp_nao_respondeu = 0;
uint8_t _potencia_nao_respondeu;
uint16_t rpm;
float tensao, corrente, temperatura;
void enviar_mensagem_supervisor(char *msg);
uint16_t soltou_correia();

int main(void) {
	//Alerta, LED
	set_bit(DDRD, PD7);
	ALERT_ON;

	//Inicializando
	uart_init();
#if DEBUG
	uart_sendSTR("Inciando\n");
#endif
	spi_init_master();
	inicializarFreio();

	//Testar
	frear();
	_delay_ms(1000);
	desfreiar();
	_delay_ms(1000);

	//Correia
	clr_bit(DDRD,PD6);

	ALERT_OFF;
	while(1) {
		_rpm_nao_respondeu = ler_agente_RPM(&rpm);
		_temp_nao_respondeu = ler_agente_temperatura(&temperatura);
		_potencia_nao_respondeu = ler_agente_potencia(&corrente, &tensao);

		if (soltou_correia()) {
			enviar_mensagem_supervisor("Estorrou correia");
		}

		//////////TEMPORARIO - TESTE DO FREIO E ALERTA//////
		if (rpm > 800) {
			ALERT_ON;
			if (rpm > 1000) {
				frear();
			}
		} else if ( rpm < 800) {
			ALERT_OFF;
			desfreiar();
		}
		if (_rpm_nao_respondeu > 1) {
			enviar_mensagem_supervisor("Sensor de RPM não está respondendo");
		}
		if (_temp_nao_respondeu > 1) {
			enviar_mensagem_supervisor("Sensor de temperatura não está respondendo");
		}
		if (_potencia_nao_respondeu > 1) {
			enviar_mensagem_supervisor("Sensor de potência não está respodendo");
		}
		/////////////////////////////////////////////////////
#if DEBUG
		char saida[sizeof(uint16_t)];
		//TEMPERATURA
		sprintf(saida,"%d", ((uint16_t) temperatura));
		uart_sendSTR("Temperatura: ");
		uart_sendSTR(saida);
		uart_send(' ');

		//TENSAO
		sprintf(saida,"%d", ((uint16_t) tensao));
		uart_sendSTR("Tensao: ");
		uart_sendSTR(saida);
		uart_send(' ');

		//CORRENTE
		sprintf(saida,"%d", ((uint16_t) corrente));
		uart_sendSTR("Corrente: ");
		uart_sendSTR(saida);
		uart_send(' ');

		//RPM
		sprintf(saida,"%d", ((uint16_t) rpm));
		uart_sendSTR("RPM: ");
		uart_sendSTR(saida);
		uart_send(' ');
		uart_sendSTR("\n----------\n");
		_delay_ms(250);
#else
		enviar_RPM_supervisorio();
		enviar_potencia_supervisorio();
		enviar_temp_supervisorio();
#endif
	}
}

uint16_t soltou_correia() {
	if (!tst_bit(PIND, 6)) {
		return 1;
	}
	return 0;
}

void enviar_mensagem_supervisor(char *msg) {
	uart_sendSTR("[m,");
	uart_sendSTR(msg);
	uart_send(']');
}
