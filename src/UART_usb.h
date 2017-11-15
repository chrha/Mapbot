#define F_CPU 14745600 // set clock frekvens after pdf
#define BAUD 115200
#define UDDRV ((F_CPU)/(16*BAUD)-1)
void UART_usb_init (void);
void UART_usb_transmitByte (unsigned char data);
unsigned char UART_usb_recieveByte(void);
void UART_usb_init (void)
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
	UCSR0C |= (1 << UCSZ00)|(1 << UCSZ01); // select UCSR0C and set data bits to 8 did not find URSEL
	
	
	
}	
void UART_usb_transmitByte (unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0))){} // UDRE is 1 then there is data in buffet
	UDR0 = data; // send data to data register
	
}
unsigned char UART_usb_recieveByte(void)
{
	while(!((UCSR0A) & (1 << RXC0))); // check if we receive something
	
	return UDR0;
	
	
}