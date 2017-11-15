
 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "USART_usb.h"
#include "UART_sensor.h"

#define BAUD 115200
#define F_CPU 14745600 // set clock frekvens after pdf
#define UDDRV ((F_CPU)/(16*BAUD)-1)

volatile unsigned char dtemp;
volatile unsigned char data_received_usb;
volatile unsigned char data_received_sensor;

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
	
	
	UART_usb_transmitByte(data_received);

	
}
ISR(USART1_RX_vect){
	data_received_sensor = UDR1;
}
