#include "alt.h"

void spi_cmd_send(char cmd)
{
	SPDR = cmd;
	while (bit_is_clear(SPSR, 7));	// wait until command has been sent
}

void cmd_reset(void)
{
	csb_lo();
	spi_cmd_send(CMD_RESET);
	_delay_ms(3);
	csb_hi();
}

int32_t cmd_adc(char cmd)
{
	int8_t ret;
	int32_t temp;

	temp = 0;
	
	csb_hi();
	spi_cmd_send(CMD_ADC_CONV + cmd);
	
	// delays based on command sent
	switch (cmd & 0x0f)
	{
		case CMD_ADC_256	:	_delay_us(900);
		case CMD_ADC_512	:	_delay_ms(3);
		case CMD_ADC_1024	:	_delay_ms(4);
		case CMD_ADC_2048	:	_delay_ms(6);
		case CMD_ADC_4096	:	_delay_ms(10);
	}

	
	// pull csb high and then low to finish conversion
	csb_lo();
	_delay_ms(2);
	csb_hi();

	// sends ADC read command
	spi_cmd_send(CMD_ADC_READ);
	
	spi_cmd_send(0x00);		// 1st byte
	ret = SPDR;
	temp = 65536 * ret;

	spi_cmd_send(0x00);		// 2nd byte
	ret = SPDR;
	temp = temp + 256 * ret;

	spi_cmd_send(0x00);		// 3rd byte
	ret = SPDR;
	temp = temp + ret;

	csb_lo();	// finished conversion
	
	return temp;
}

uint16_t cmd_prom(int coef_num)
{
	uint8_t ret;
	uint16_t rC;

	rC = 0;

	csb_hi();
	spi_cmd_send(CMD_PROM_RD + coef_num*2);
	spi_cmd_send(0x00);
	ret = SPDR;
	rC = 256 * ret;

	spi_cmd_send(0x00);
	ret = SPDR;

	rC = rC + ret;
	csb_lo();

	return rC;
}
