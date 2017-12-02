#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "UART_USB.h"
#include "UART_sensor.h"

void pwm_init();
void pwm_drive();
void rotate_left();
void rotate_right();
void dir_pid_foreward(signed char pid);
void dir_foreward(void);
void dir_backward();
void where_to_go(void);
signed char pid=0;
uint8_t temp0;
uint8_t temp1;
uint8_t temp2;
uint8_t temp3;
uint8_t temp4;
const unsigned char const_speed = 150;
int main(void)
{
	DDRA =0x00;
	DDRD=0xff;
	pwm_init();
	UART_init();
	sei();
    while(1)
    {
	
		
		where_to_go();
						
    }
}

ISR(USART1_RX_vect){
	pid = UDR1;
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

	OCR3B = const_speed; // left
	OCR3A = const_speed; // right

}
void dir_pid_foreward(signed char pid)
{
	
	OCR3A = const_speed + pid;
	OCR3B = const_speed - pid;
	PORTD |= (1 << PORTD1); // Pin PD0 goes high (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes high (forward_left)
	
	
}

void dir_foreward(){
	
	PORTD |= (1 << PORTD1); // Pin PD0 goes high (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes high (forward_left)
	pwm_drive();
}

void dir_backward()
{
	PORTD &= ~(1 << PORTD1); // Pin PD0 goes low (forward_right)
	PORTD &= ~(1 << PORTD0); // Pin PD1 goes low (forward_left)
	pwm_drive();
}
void rotate_right()
{
	PORTD &= ~(1 << PORTD1); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes low (forward_left)
	pwm_drive();
}
void rotate_left()
{
	PORTD &= ~(1 << PORTD0); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD1); // Pin PD1 goes low (forward_left)
	pwm_drive();
}

void where_to_go(void)
{
	
	temp0= PINA & 0b00000111;

	
	if(temp0 == 1){
		
		rotate_right();
		
	}
	else if(temp0 == 2){
		rotate_left();
		
	}
	else if(temp0 == 3){
		dir_pid_foreward(pid);
			
	}
	else if(temp0 == 4){
		dir_backward();
			
	}else if(temp0 == 5){
		dir_foreward();
	
	}
	else{
		OCR3B = 0; 
		OCR3A = 0;
	}
}