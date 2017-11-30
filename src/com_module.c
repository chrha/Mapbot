#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "UART_USB.h"
#include "UART_sensor.h"

volatile unsigned char data_received_usb;
volatile unsigned char data_received_sensor;
signed char KP=6;
signed char KI=0;
signed char KD=3; 
volatile unsigned char sensor1=0;
volatile unsigned char sensor2=0;
volatile int i=0;
signed char latest_error=0;
signed char I=0;
signed char P;
signed char desired_value=0;
signed char actual_value;
signed char D;
signed char PID_value;
signed char error;
signed char strong1=30;
signed char strong2=-20;
signed char weak1=10;
signed char weak2=-5; 

volatile uint8_t temp0;

volatile uint8_t temp1;

void terminal_view (uint8_t adc_channel1,uint8_t adc_channel2, uint8_t actual);

void PID(void);

int main(void)
{
	
	DDRB = 0xFF;
	DDRA =0x00;
	
	UART_init();
	UART_usb_init();
	sei();
	while(1)
	{
		
		PORTB |= (1 << PINB4);
		PORTB &= ~(1 << PINB2);
		PORTB &= ~(1 << PINB3);
		PORTB &= ~(1 << PINB5);
		_delay_ms(2);
		
		PID();
		
		
	}
	
	
}
ISR(USART0_RX_vect){
	data_received_usb= UDR0;
	
	
}
ISR(USART1_RX_vect){
	data_received_sensor = UDR1;
	
	temp0 = PINA & 0b00000001;
	temp1 = PINA & 0b00000010;
	if(temp0 == 1){
		sensor1= data_received_sensor;
		
	}
	else if(temp1 == 2){
		sensor2=data_received_sensor;
	}
	


}
void PID(){
	

		
	
	actual_value= sensor1- sensor2;
	error= desired_value - actual_value;
		
	P= error;
	I += error;
	D = (error - latest_error);
	if(I >= 100){  // limit wind up
		I=100;
	}
	PID_value= KP*P + KD* D + KI*I;
	
	latest_error=error;
	
	if((sensor1 > 181) && (sensor2 < 181) ){
		
		UART_transmitByte((weak2+PID_value)); // lugnt
		
	}else if( (sensor1 > 181) && ( sensor2 > 181)){
		
		UART_transmitByte((strong2+PID_value)); //stark
		
	}else if((sensor1 < 152) && (sensor2 > 152)){ // 152
		
		UART_transmitByte((weak1+PID_value));  //lugn
		
	}else if((sensor1 < 152) && (sensor2 < 152)){
		
		UART_transmitByte((strong1+ PID_value)); //stark
		
	}else{
		UART_transmitByte(PID_value);
	}
		
	
	
	

	
}
