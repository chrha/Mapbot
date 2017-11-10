/*
 * UART_transm.c
 *
 * Created: 11/10/2017 9:07:26 AM
 *  Author: nisan016
 */ 


#include <avr/io.h>
#define F_CPU 8000000 // set clock frekvens after pdf
#include "delay.h"
#include <stdio.h>
#define BAUD 9600
#include <util/setbaud.h>


void UART_init (void);
void UART_transmitByte (unsigned char data);
unsigned char UART_recieveByte(void);

int main(void)
{
	//DDRB = 0b00000001;
	UART_init();
    while(1)
    {
        //TODO:: Please write your application code
		UART_transmitByte('a');
		//PORTB |= (1<<PINB0);
		_delay_ms(500);
		
		
    }
}

void UART_init (void)
{
	// Set all needed register
	PRR0 &= ~(1<<PRUSART0);
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRH_VALUE;
	#if USE_2X
	UCSR0A |= (1<<U2X0);
	#else
	UCSR0A &= ~(1<<U2X0);
	#endif
	UCSR0B |= ((1 << TXEN0)|(1 << RXEN0)|(0<<UCSZ02)|(0<<RXCIE0)); // set transmitter enable and receiver enable to 1 to indicate UART is ready to RX and TX
	UCSR0C |= (1 << UCSZ00)|(1 << UCSZ10); // select UCSR0C and set data bits to 8 did not find URSEL
	
	
	
}	

void UART_transmitByte (unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0))){} // UDRE is 1 then there is data in buffet
	UDR0 = data; // send data to data register
	
}

unsigned char UART_recieveByte(void)
{
	while(!((UCSR0A) & (1 << RXC0))); // check if we receive something
	
	return UDR0;
	

}