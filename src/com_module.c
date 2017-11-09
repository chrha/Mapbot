/*
 * GccApplication1.c
 *
 * Created: 11/6/2017 3:58:21 PM
 *  Author: iliay038
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>




//#define BAUDRATE ((F_CPU)/ (UART_BAUD* 16UL)-1) // equation for calculating UBRR value
void uart_init(void); 
void transmitt_data(unsigned char data); 
unsigned char receive_data(void);

int main(void)
{
	DDRB |= (1<<PINB0);
	PORTB = ~(1 << PINB0);
	uart_init();
	//receive_data();
	
    while(1)
    {
		transmitt_data('a');
		_delay_ms(5000); 
		//receive_data();
		//uartrecieve();
	
		//unsigned char value = receive_data();
        //TODO:: Please write your application code 
    }
}


/**Initializes UART connection **/
void uart_init(void){
	#define BAUD 9600
	//#define F_CPU 8000000
	#include <util/setbaud.h>
	UBRR0L=UBRRL_VALUE;			// set baud rate
	UBRR0H= UBRRH_VALUE;	// shift register 8 bits to define left byte of the 2byte register
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);	// enable transmitter and receiver
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);  // Set data size of 8 bit
	
}

/** waits until buffer UDR isn't empty,
 then loads data to register **/
void transmitt_data(unsigned char data){
	
		
	
		
		 while (!(UCSR0A&(1<<UDRE0)));    // Wait for previous transmissions
		 UDR0 = data;    // Send data byte
	
	
}


/** waits until there is no longer anything to receive,
 then returns the value **/
unsigned char receive_data(void){
	
	while(!(UCSR0A & (1<<RXC0))){
		PORTB ^= (1 << PINB0);
	}
	
	return UDR0;
}
