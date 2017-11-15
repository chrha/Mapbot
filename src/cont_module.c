/*
 * PWM.c
 *
 * Created: 11/15/2017 11:54:50 AM
 *  Author: andma590
 */ 


#include <avr/io.h>

void pwm_init();
void pwm_drive();
void rotate_left();
void rotate_right();
void dir_foreward();
void dir_backward();
void where_to_go(void);

int main(void)
{
	DDRA =0x00;
	DDRD=0xff;
    while(1)
    {
        pwm_init();
		rotate_right();
		
    }
}

void pwm_init()
{
	TCCR3A = 0b10100001; //COM3A1 = 1, COM3B1 = 1, WGM30 = 1;
	TCCR3B = 0b00000011; // CS31 = , CS30 = 1
	TCNT0 = 0;
	DDRB = 0xff; 	
}

void pwm_drive()
{
	
	OCR3B = 150; // left
	OCR3A = 150; // right
	
}

void dir_foreward()
{
	PORTD &= ~(1 << PORTD1); // Pin PD0 goes low (forward_right)
	PORTD &= ~(1 << PORTD0); // Pin PD1 goes low (forward_left)
	pwm_drive();	
}

void dir_backward()
{
	PORTD |= (1 << PORTD1); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes low (forward_left)
	pwm_drive();
}


void rotate_left()
{
	PORTD &= ~(1 << PORTD1); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes low (forward_left)
	pwm_drive();
}

void rotate_right()
{
	PORTD &= ~(1 << PORTD0); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD1); // Pin PD1 goes low (forward_left)
	pwm_drive();
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
		dir_foreward();
		
	}
	else if(temp3 == 0x08){
		dir_backward();
		
		
	}