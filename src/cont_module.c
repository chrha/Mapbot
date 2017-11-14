/*
 * Robot_project_steering.c
 *
 * Created: 11/8/2017 2:50:57 PM
 *  Author: andma590
 */ 


#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 14745600
void forward ();
void rotate_left();
void rotate_right();
void backward();



	
int main(void)
{
	
DDRD|=(0xFF); // Setting all ports(D) to output
DDRC|=

	
}

void forward()
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

void rotate_left()
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

void rotate_right()
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

void backward()
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

void where_to_go()
{
	
	
	
	
}