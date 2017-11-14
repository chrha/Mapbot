
/*
 * UART_transm.c
 *
 * Created: 11/10/2017 9:07:26 AM
 *  Author: nisan016
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define F_CPU 14745600 // set clock frekvens after pdf
#define UDDRV ((F_CPU)/(16*BAUD)-1)
//#include <util/setbaud.h>
void UART_init (void);
void UART_transmitByte (unsigned char data);
unsigned char UART_recieveByte(void);
unsigned char str[12] = "420 blaze it";
unsigned int i=0;
volatile unsigned char data_received;
int main(void)
{
	//DDRB = 0b00000001;
	DDRB |= (1 << PINB2);
	PORTB &= ~(1 << PINB2);
	
	UART_init();
	sei();
    while(1)
    {
		
        //TODO:: Please write your application code
		
		
		
		
		
    }
}
ISR(USART0_RX_vect){
	data_received= UDR0;
	
	if (data_received == 'a'){
		PORTB |= (1<<PINB2);
	}
	
	if(data_received == 'd'){
		PORTB |= (1<<PINB3);
	}
	if(data_received == 'w') {
		PORTB |= (1<<PINB4);
	}
	if(data_received == 's'){
		PORTB |= (1<<PINB5);
	}
	UART_transmitByte(data_received);
	data_received='0';
	
	PORTB |= (0<<PINB2);
	PORTB |= (0<<PINB3);
	PORTB |= (0<<PINB4);
	PORTB |= (0<<PINB5);
	
}
void UART_init (void)
{
	// Set all needed register
	//PRR0 &= ~(1<<PRUSART0);
	UBRR0H = (UDDRV>>8);
	UBRR0L = UDDRV;
	#if USE_2X
	UCSR0A |= (1<<U2X0);
	#else
	UCSR0A &= ~(1<<U2X0);
	#endif
	UCSR0B |= ((1 << TXEN0)|(1 << RXEN0)|(1<<RXCIE0)); // set transmitter enable and receiver enable to 1 to indicate UART is ready to RX and TX
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