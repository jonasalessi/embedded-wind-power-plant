/*
 * spi.h
 *
 *  Created on: May 27, 2014
 *      Author: jonas
 */

#ifndef SPI_H_
#define SPI_H_

#include "../utils.h"

#define MOSI         PB3
#define MISO         PB4
#define SCK          PB5

#define CS_POTENCIA  PB2
#define CS_TEMPERATURA PB0
#define CS_RPM PB6

void spi_init_master();
char spi_transmit(char dado);

#endif /* SPI_H_ */
