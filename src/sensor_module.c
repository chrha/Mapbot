/*
 * GccApplication3.c
 *
 * Created: 11/15/2017 1:01:13 PM
 *  Author: andma590
 */ 


#include <avr/io.h>
#define F_CPU 14745600
#include <math.h>
#include <util/delay.h>
#include "UART_sensor.h"
void adc_init();
uint16_t adc_read(uint8_t ch);
//void testbench(uint8_t adc_value);
uint8_t cm_converter(uint16_t adc);
 uint8_t adc_value;
 char* casting (uint8_t adc_to_cast);

int main(void)
{
	
	adc_init();
	UART_init();
    while(1)
    {
		adc_value= adc_read(0);
		uint8_t ental, tiotal, hundratal;
		
		ental = (adc_value)%10;
		tiotal = (adc_value/10)%10;
		hundratal = (adc_value/100)%10;
		char string[4]= {hundratal+'0',tiotal+'0',ental+'0','\0'};
		for (int i = 0; i<4; i++)
		{
			UART_transmitByte(string[i]);
			_delay_ms(50);
			
		}
		
    }
}

void adc_init()
{
	//voltage ref only
	//Prescaler 14745600/128=115200
	// ADEN = ADC enable , ADPSx = prescaler selector
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

}

uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with ’7′ will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1′ to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0′ again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return cm_converter(ADC);
}


uint8_t cm_converter(uint16_t adc)
{
	return (uint8_t)27.6649/(pow(adc*5/1023, 1.182033096926714));
	
}

 char* casting (uint8_t adc_to_cast)
{
	char* string;
	uint8_t ental, tiotal, hundratal;
	
	ental = (adc_to_cast)%10;
	tiotal = (adc_to_cast/10)%10;
	hundratal = (adc_to_cast/100)%10;
	
	string[0] ='k';
	string[1] = 'u';
	string[2] =  'k';
	return string;
}

/*void testbench(uint8_t adc_value)
{
	if (adc_value >= 20 && adc_value <=50)
	{
		PORTD |= (1 << PORTD4); // pin 18
	}
	else
	{
		PORTD &= ~(1 << PORTD4); // pin 18
	}
	
	*/
	
