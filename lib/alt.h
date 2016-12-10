#ifndef ALT_H
#define ALT_H

#define TRUE 1
#define FALSE 0

#define F_CPU 	8000000UL
#define P0		1013.25

#define CMD_RESET		0x1E
#define CMD_ADC_READ 	0x00
#define CMD_ADC_CONV	0x40
#define CMD_ADC_D1		0x00
#define CMD_ADC_D2		0x10
#define CMD_ADC_256		0x00
#define CMD_ADC_512		0x02
#define CMD_ADC_1024	0x04
#define CMD_ADC_2048	0x06
#define CMD_ADC_4096	0x08
#define CMD_PROM_RD		0xA0


// CHANGE FOLLOWING DEFINITIONS TO ACTUAL CS PIN
#define csb_hi()		(PORTB &= ~(1 << PB2))// setting SS low
#define csb_lo()		(PORTB |= (1 << PB2))// setting SS high

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void spi_cmd_send(
	char cmd
	);


void cmd_reset(
	void
	);

unsigned long cmd_adc(
	char cmd
	);

unsigned int cmd_prom(
	char coef_num
	);

double read_sensor(
	void
	);

double altitude(
	void
	);

#endif