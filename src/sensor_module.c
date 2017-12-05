#include <avr/io.h>
#define F_CPU 14745600
#include <math.h>
#include <util/delay.h>
#include "UART_sensor.h"
#include "I2C.h"
#include <stdio.h>
#include <string.h>
void adc_init();
uint8_t adc_read(uint8_t ch);
void SendString(char mydata[20]);
void terminal_view (uint8_t adc_channel1, uint8_t adc_channel2);
uint8_t temp0, temp1;
void what_to_send(void);
uint16_t s1=0;
uint16_t s2=0;
signed char actual_value=0;
void sensor_right_front(void);
void sensor_right_back(void);
void sensor_front(void);
void sensor_left(void);
void sensor_gyro(void);
void sensor_distance(void);

uint8_t lenght_right();
uint8_t lenght_left();
uint8_t black_4=0;
uint8_t white_4=0;
uint8_t black_5=0;
uint8_t white_5=0;
uint8_t PB_4=0;
uint8_t PB_5=0;
uint8_t latest_color1=0;
uint8_t latest_color2=0;
float average_distance=0;
float left_distance=0;
float right_distance=0;
uint8_t average_front_distance=0;
uint8_t temp=0;
int main(void)
{
	DDRB = 0b01001111;
	DDRD = 0x00;
	PORTB &= 0b00000000;
	adc_init();
	UART_init();
	I2C_init();
	I2C_power();
	
	while(1)
	{
		//s1= (uint16_t)(floor(10*(27.6649/pow((adc_read(1)*2.5/255),(1.182033096926714)))));
		//s2= (uint16_t)(floor(10*(27.6649/pow((adc_read(0)*2.5/255),(1.182033096926714)))));
		//actual_value= (signed char)((uint8_t)(s1-s2));
		
		// notera att PINB4 inte funkar.enbart jordning.
		
		temp = PIND & 0b10000000;
		if(temp == 0b10000000)
		{
			black_4=0;
			black_5=0;
			white_4=0;
			white_5=0;
		}
		
		sensor_distance();
		
		sensor_right_front();
		sensor_right_back();
		
		lenght_left();
		lenght_right();
		
		sensor_front();
		sensor_left();
		
		lenght_left();
		lenght_right();
		
		sensor_gyro();
		
		
		

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

void terminal_view (uint8_t adc_channel1, uint8_t adc_channel2)
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


void sensor_right_front(){
	PORTB |= (1 << PINB0);
	PORTB &= 0b00000001;
	UART_transmitByte(adc_read(1));
	_delay_ms(5);
}

void sensor_right_back(){
	PORTB |= (1 << PINB1);
	PORTB &= 0b00000010;
	UART_transmitByte(adc_read(0));
	_delay_ms(5);
}

void sensor_front(){
	average_front_distance= ( adc_read(3)+adc_read(4) ) / 2;
	PORTB |= 0b00000011;
	PORTB &= 0b00000011;
	UART_transmitByte(average_front_distance);
	_delay_ms(5);
}


void sensor_left(){
	
	PORTB |= (1 << PINB2);
	PORTB &= 0b00000100;	
	UART_transmitByte(adc_read(2));
	_delay_ms(5);
}

void sensor_gyro(){
	
	PORTB |= 0b00000101;
	PORTB &= 0b00000101;
	UART_transmitByte(I2C_receive(0x2C)); // low
	_delay_ms(5);
	
	PORTB |= 0b00000110;
	PORTB &= 0b00000110;
	UART_transmitByte(I2C_receive(0x2D)); // high
	_delay_ms(5);
}

void sensor_distance(void){
	left_distance=(float)lenght_left();
	right_distance= (float)lenght_right();
	average_distance = (left_distance + right_distance) / 2;
	
	
	if ( average_distance >= 24){	// ha 24. 28 på prick. 30 stor.
		
		PORTB |= 0b00000111;
		PORTB &= 0b00000111;
		UART_transmitByte('1');
		black_4=0;
		black_5=0;
		white_4=0;
		white_5=0;
	}
	else{
		PORTB = 0b00000000;
	}
	_delay_ms(5);
}  
 
uint8_t lenght_left()
{
	PB_4 = PINB & 0b00010000;
	
	
	// black = low, white =high
	if ((PB_4 == 0b00010000) && !(latest_color1==0b00010000))
	{
		white_4++;
		latest_color1 = PB_4;
		
	}
	else if ((PB_4 == 0b00000000) && !(latest_color1 == 0b00000000))
	{
		black_4++;
		latest_color1=PB_4;
	}
	
	return black_4 + white_4;
}


uint8_t lenght_right()
{
	PB_5 = PINB & 0b00100000;
	
	
	// black = low, white =high
	if ((PB_5 == 0b00100000) && !(latest_color2==0b00100000))
	{
		white_5++;
		latest_color2 = PB_5;
		
	}
	else if ((PB_5 == 0b00000000) && !(latest_color2 == 0b00000000))
	{
		black_5++;
		latest_color2=PB_5;
	}
	
	return black_5 + white_5;
}