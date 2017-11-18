#include <avr/io.h>
#define F_CPU 14745600
#include <math.h>
#include <util/delay.h>
#include "UART_sensor.h"
#include <stdio.h>
#include <string.h>

void adc_init();
unsigned char adc_read(uint8_t ch);
void SendString(char mydata[20]);
void terminal_view (uint8_t adc_channel);


int main(void)
{
	
	adc_init();
	UART_init();
    while(1)
    {
		terminal_view(0);

    }
}

void adc_init()
{
	ADMUX|=0b11100000;
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

}

unsigned char adc_read(uint8_t ch)
{

	ch &= 0b00000111; 
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	
	
	while(ADCSRA & (1<<ADSC));
	
	return ADCH;
}

void SendString(char mydata[20])
{
	int i;
	for(i=0;i<strlen(mydata);i++)
	{
		UART_transmitByte(mydata[i]);
	}
}

void terminal_view (uint8_t adc_channel)
{
	adc_channel&=0b00000111;
	char mychar[5];
	
	SendString("ADC_value: ");
	sprintf(mychar, "%04d", adc_read(adc_channel));
	UART_transmitByte(mychar[1]);
	UART_transmitByte(mychar[2]);
	UART_transmitByte(mychar[3]);
	UART_transmitByte(mychar[4]);
	UART_transmitByte(13);// newline ascii
	_delay_ms(500);

	
	
	
}