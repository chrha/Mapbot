


#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 14745600
void forward (void);
void rotate_left(void);
void rotate_right(void);
void backward(void);
void where_to_go(void);

uint8_t temp0 =0;
uint8_t temp1 =0;
uint8_t temp2 =0;
uint8_t temp3=0;

	
int main(void)
{
	
	DDRD =(0xFF); // Setting all ports(D) to output
	DDRA =(0x00); // Setting all port(C) to input
	PORTD =0x00;
	while(1){
		where_to_go();
	
	}
	
}



void forward(void)
{

	PORTD &= ~(1 << PORTD1); // Pin PD0 goes low (forward_right) 
	PORTD &= ~(1 << PORTD0); // Pin PD1 goes low (forward_left)
	
	PORTD |= (1 << PORTD4);
	PORTD |= (1 << PORTD6);
	_delay_ms(50);
	PORTD &= ~(1 << PORTD4); 
	PORTD &= ~(1 << PORTD6);
	_delay_ms(50);
	
}

void rotate_left(void)
{
	PORTD &= ~(1 << PORTD1); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes low (forward_left)
	
	PORTD |= (1 << PORTD4);
	PORTD |= (1 << PORTD6);
	_delay_ms(50);
	PORTD &= ~(1 << PORTD4);
	PORTD &= ~(1 << PORTD6);
	_delay_ms(50);
			
}

void rotate_right(void)
{
	PORTD &= ~(1 << PORTD0); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD1); // Pin PD1 goes low (forward_left)
	
	PORTD |= (1 << PORTD4);
	PORTD |= (1 << PORTD6);
	_delay_ms(50);
	PORTD &= ~(1 << PORTD4);
	PORTD &= ~(1 << PORTD6);
	_delay_ms(50);
	
}

void backward(void)
{

	PORTD |= (1 << PORTD1); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes low (forward_left)
	
	PORTD |= (1 << PORTD4);
	PORTD |= (1 << PORTD6);
	_delay_ms(50);
	PORTD &= ~(1 << PORTD4);
	PORTD &= ~(1 << PORTD6);
	_delay_ms(50);
	
}

void where_to_go(void)
{
	
	temp0= PINA & 0x01;
	temp1= PINA & 0x02;
	temp2= PINA & 0x04;
	temp3= PINA & 0x08;
	
	if(temp0 == 0x01){
		
			rotate_left();
		
	}
	else if(temp1 == 0x02){

			rotate_right();
	

	}
	else if(temp2 == 0x04){
			forward();
	
	}
	else if(temp3 == 0x08){
			backward();
		
		
	}
}