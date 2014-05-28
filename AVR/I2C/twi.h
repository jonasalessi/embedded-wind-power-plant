/*
  twi.c - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2014 by Jonas Alessi
*/
#ifndef twi_h
#define twi_h

#include <inttypes.h>

#ifndef TWI_FREQ
#define TWI_FREQ 100000L
#endif

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif

#define TWI_READY 0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4

#define SCL PC5
#define SDA PC4
#define BUFFER_LENGTH 32

void twi_init();
void twi_setAddress(uint8_t addr);
uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop);
uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop);
uint8_t twi_transmit(const uint8_t* data, uint8_t length);
void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) );
void twi_attachSlaveTxEvent(void (*function)(void));
void twi_reply(uint8_t ack);
void twi_stop();
void twi_releaseBus();
void twi_begin();
void twi_beginTransmission(uint8_t addr);
uint8_t twi_endTransmission();
uint8_t twi_write(uint8_t data);
uint8_t twi_requestFrom(uint8_t address, uint8_t quantity);
int twi_read();
int twi_available();

#endif

