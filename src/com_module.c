#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "UART_USB.h"
#include "UART_sensor.h"

#define BAUD 115200
#define F_CPU 14745600 // set clock frekvens after pdf
#define UDDRV ((F_CPU)/(16*BAUD)-1)

volatile unsigned char dtemp;
volatile unsigned char data_received_usb;
volatile unsigned char data_received_sensor;

float KP=0;
float KI=0;
float KD=0;
float latest_error=0;
float I=0;
float P;
float desired_value=0.01;
float actual_value;
float D;
float PID_value;
float error;

void PID(void);

int main(void)
{
	
	DDRB = 0xFF;
	
	sei();
	UART_init();
	UART_usb_init();
	
	while(1)
	{
		
		
	}
}
ISR(USART0_RX_vect){
	data_received_usb= UDR0;
	//PORTB &= (0<<PINB2);
	//PORTB &= (0<<PINB3);
	//PORTB &= (0<<PINB4);
	//	PORTB &= (0<<PINB5);
	
	
	UART_usb_transmitByte(data_received_usb);
	
}
ISR(USART1_RX_vect){
	data_received_sensor = UDR1;
}

void PID(){
	
	error= desired_value - actual_value;
	
	P= error;
	I += error;
	D = (error - latest_error);
	if(I >= 100){  // limit wind up
		I=100;
	}
	PID_value= KP*P + KD* D + KI*I;
	
	UART_transmitByte(PID_value);      
	
	if( PID_value < 0){              // rotate right
		PORTB |= (1 << PINB3);
		PORTB &= (0<<PINB3);
		PORTB &= (0<<PINB4);
		PORTB &= (0<<PINB5);
	}
	else if(PID_value > 0){ // rotate left
		PORTB |= (1 << PINB2);
		PORTB &= (0<<PINB2);
		PORTB &= (0<<PINB4);
		PORTB &= (0<<PINB5);
		
	}
	else{
		PORTB |= (1 << PINB4);   // forward
		PORTB &= (0<<PINB3);
		PORTB &= (0<<PINB2);
		PORTB &= (0<<PINB5);
	}
	latest_error=error;
	
	
}