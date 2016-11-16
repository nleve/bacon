#ifndef ALT_H
#define ALT_H

#define TRUE 1
#define FALSE 0

#define F_CPU 8000000UL

#define CMD_RESET		0x1E
#define CMD_ADC_READ 	0x00
#define CMD_ADC_CONV	0x40
#define CMD_ADC_D1		0x00
#define CMD_ADC_D1		0x10
#define CMD_ADC_256		0x00
#define CMD_ADC_512		0x02
#define CMD_ADC_1024	0x04
#define CMD_ADC_2048	0x06
#define CMD_PROM_RD		0xA0


// CHANGE FOLLOWING DEFINITIONS TO ACTUAL CS PIN
#define csb_hi()		(PORTB &= ~(1 << PB5))// setting csb low
#define csb_lo()		(PORTB |= (1 << PB5))// setting csb high

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void spi_command_send(
	char cmd
	);


void command_reset(
	void
	);

unsigned long cmd_adc(
	char cmd
	);

unsigned int cmd_prom(
	char coef_num
	);

unsigned int crc4(
	unsigned int n_prom[]
	);

#endif
