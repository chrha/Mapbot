#include <avr/io.h>
#define F_CPU 14745600
#include <math.h>
#include <util/delay.h>
#include "UART_sensor.h"

#include <stdio.h>
#include <string.h>
void adc_init();
uint8_t adc_read(uint8_t ch);
void SendString(char mydata[20]);
void terminal_view (int8_t adc_channel1, uint8_t adc_channel2);
uint8_t temp0, temp1;
void what_to_send(void);
uint16_t s1=0;
uint16_t s2=0;
signed char actual_value=0;
int main(void)
{
	DDRB = 0xFF;
	adc_init();
	UART_init();
	
	
	while(1)
	{
		//s1= (uint16_t)(floor(10*(27.6649/pow((adc_read(1)*2.5/255),(1.182033096926714)))));
		//s2= (uint16_t)(floor(10*(27.6649/pow((adc_read(0)*2.5/255),(1.182033096926714)))));
		//actual_value= (signed char)((uint8_t)(s1-s2));
		
		PORTB |= (1 << PINB0);
		PORTB &= ~(1 << PINB1);
		UART_transmitByte(adc_read(1));
		_delay_ms(10);
		PORTB |= (1 << PINB1);
		PORTB &= ~(1 << PINB0);
		UART_transmitByte(adc_read(0));
		_delay_ms(10); 
		
		//uint8_t Low =  I2C_receive(0x2C);
		//uint8_t High = I2C_receive(0x2D);
		
		//terminal_view(1,0);
		
	}
}
void adc_init()
{
	ADMUX|=0b11100000;
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
uint8_t adc_read(uint8_t ch)
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

void terminal_view (int8_t adc_channel1, uint8_t adc_channel2)
{
	adc_channel1&=0b00000111;
	adc_channel2&=0b00000111;
	char mychar1[5];
	char mychar2[5];

	SendString("ADC_value1: ");
	sprintf(mychar1, "%04d", adc_read(adc_channel1));
	UART_transmitByte(mychar1[1]);
	UART_transmitByte(mychar1[2]);
	UART_transmitByte(mychar1[3]);
	UART_transmitByte(mychar1[4]);
	_delay_ms(50);
	
	SendString("   ADC_value2: ");
	sprintf(mychar2, "%04d", adc_read(adc_channel2));
	UART_transmitByte(mychar2[1]);
	UART_transmitByte(mychar2[2]);
	UART_transmitByte(mychar2[3]);
	UART_transmitByte(mychar2[4]);
	UART_transmitByte(13);
	_delay_ms(50);
	
	
	
}