#define F_CPU 14745600 // set clock frekvens after pdf
#define BAUD 115200
#define UDDRV ((F_CPU)/(16*BAUD)-1)
void UART_init (void);
void UART_transmitByte (unsigned char data);
unsigned char UART_recieveByte(void);
void UART_init (void)
{
	// Set all needed register
	//PRR0 &= ~(1<<PRUSART0);
	UBRR1H = (UDDRV>>8);
	UBRR1L = UDDRV;
	#if USE_2X
	UCSR1A |= (1<<U2X1);
	#else
	UCSR1A &= ~(1<<U2X1);
	#endif
	UCSR1B |= ((1 << TXEN1)|(1 << RXEN1));//|(0<<UCSZ02)|(0<<RXCIE0)); // set transmitter enable and receiver enable to 1 to indicate UART is ready to RX and TX
	UCSR0C |= (1 << UCSZ10)|(1 << UCSZ11); // select UCSR0C and set data bits to 8 did not find URSEL
	
	
	
}	
void UART_transmitByte (unsigned char data)
{
	while(!(UCSR1A & (1 << UDRE1))){} // UDRE is 1 then there is data in buffet
	UDR1 = data; // send data to data register
	
}
unsigned char UART_recieveByte(void)
{
	while(!((UCSR1A) & (1 << RXC1))); // check if we receive something
	
	return UDR1;
	
	
}