#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "UART_USB.h"
#include "UART_sensor.h"
volatile unsigned char data_received_usb;
volatile unsigned char data_received_sensor;
signed char KP=6;
signed char KI=0;
signed char KD=3;
volatile unsigned char sensor_right_front=0;
volatile unsigned char sensor_right_back=0;
volatile unsigned char sensor_front=0;
volatile unsigned char sensor_left=0;
volatile unsigned char sensor_gyro_low=0;
volatile unsigned char sensor_gyro_high=0;
volatile unsigned char sensor_distance=0;


volatile int i=0;
signed char latest_error=0;
signed char I=0;
signed char P;
volatile signed char desired_value=0;
volatile signed char actual_value;
signed char D;
signed char PID_value;
volatile signed char error=0;
signed char strong1=30;
signed char strong2=-20;
signed char weak1=10;
signed char weak2=-5; // -10
volatile uint8_t temp0;
volatile uint8_t temp1;
volatile uint8_t count_walls=1;
void rotate_right(void);
void rotate_left(void);
void pid_forward(void);
void backward(void);
void forward(void);
void stop_servos(void);
void rotate_90_left(void);
void rotate_90_right(void);
double angle = 0;
double tempG;
double gyroData = 0;
char k=0;
void PID(void);
int main(void)
{
	
	DDRB = 0xFF;
	DDRD = 0xFF;
	DDRA =0x00;
	PORTD  &= 0b00000000;
	UART_init();
	UART_usb_init();
	stop_servos();
	sei();	
	while(1)
	{
			
		pid_forward();

	
		
		if (error >= 55){
			forward();
			_delay_ms(4000);
			stop_servos();
			_delay_ms(10000);
			rotate_90_right();
			_delay_ms(10000);
			stop_servos();
			_delay_ms(10000);
			forward();
			_delay_ms(10000);
			
		}else if ((error <=55) && (sensor_front >= 110) && (sensor_left >=120)){
			stop_servos();
			_delay_ms(10000);
			rotate_90_left();
			stop_servos();
			_delay_ms(10000);
			rotate_90_left();
			stop_servos();
			_delay_ms(10000);
		
			while(sensor_left >= 181){
				if (error >= 55){
					forward();
					_delay_ms(4000);
					stop_servos();
					_delay_ms(5000);
					rotate_90_right();
					_delay_ms(10000);
					stop_servos();
					_delay_ms(5000);
					forward();
					_delay_ms(10000);
					stop_servos();
				
				}else{
					pid_forward();
				}
			
			}
		
		
		} else if ((error <=55) && (sensor_front >= 110)){
			stop_servos();
			_delay_ms(10000);
			rotate_90_left();
			stop_servos();
			_delay_ms(10000);
		} 
	

			  // 43(ca 80 cm) stanna om ö hittad
	
		
	
		
	
		
		
	
	
	
			
	
	}
	
	
}

ISR(USART0_RX_vect){
	
	data_received_usb= UART_usb_recieveByte();

		
}

ISR(USART1_RX_vect){
	
	data_received_sensor = UDR1;

	
	temp0 = PINA & 0b00000111;
	
	if(temp0 == 1){
		sensor_right_front= data_received_sensor;
		
	}
	else if(temp0 == 2){
		sensor_right_back=data_received_sensor;
	}else if (temp0 == 3){
		sensor_front=data_received_sensor;
	}else if (temp0 == 4){
		sensor_left= data_received_sensor;
	}else if (temp0 == 5){
		sensor_gyro_low= data_received_sensor;
	}else if (temp0 == 6){
		sensor_gyro_high= data_received_sensor;
	}else if(temp0 == 7){
		sensor_distance=data_received_sensor;
		count_walls +=1;
	
			
	}
	
	
	gyroData = (int16_t)(sensor_gyro_low | (int16_t)(sensor_gyro_high <<8));
	gyroData *= 0.07;
	


	
	char mychar1[4];
	sprintf(mychar1, "%04d", count_walls);
	UART_usb_transmitByte(mychar1[1]);
	UART_usb_transmitByte(mychar1[2]);
	UART_usb_transmitByte(mychar1[3]);	
	UART_usb_transmitByte(13); 
		
	
	
	
}
void PID(){
	
	
	
	actual_value= sensor_right_front- sensor_right_back;
	error= desired_value - actual_value;
	
	
	P= error;
	I += error;
	D = (error - latest_error);
	if(I >= 100){  // limit wind up
		I=100;
	}
	PID_value= KP*P + KD* D + KI*I;
	
	latest_error=error;
	
	if((sensor_right_front > 159) && (sensor_right_back < 159) ){
		
		UART_transmitByte((weak2+PID_value)); // lugnt
		
		}else if( (sensor_right_front > 159) && ( sensor_right_back > 159)){
		
		UART_transmitByte((strong2+PID_value)); //stark
		
		}else if((sensor_right_front < 138) && (sensor_right_back > 138)){ // 152
		
		UART_transmitByte((weak1+PID_value));  //lugn
		
		}else if((sensor_right_front < 138) && (sensor_right_back < 138)){
		
		UART_transmitByte((strong1+ PID_value)); //stark
		
		}else{
		UART_transmitByte(PID_value);
	}
	
	
}

void rotate_left(){
	
	PORTB |= (1 << PINB2);
	PORTB &= 0b00000100;
	
	
}

void rotate_right(){
	
	PORTB |= (1 << PINB3);
	PORTB &= 0b00001000;
	
}

void stop_servos(){
	PORTB &= 0b00000000;
}

void pid_forward(){
	
	PORTB |= 0b00001100;
	PORTB &= 0b00001100;
	_delay_ms(10);
	PID();
	_delay_ms(10);
}

void backward(){
	
	PORTB |= 0b00010000;
	PORTB &= 0b00010000;
	_delay_ms(10);
}

void forward(){
	
	PORTB |= 0b00010100;
	PORTB &= 0b00010100;
	_delay_ms(10);
}

void rotate_90_right(void){
	rotate_right();
	PORTD |= 0b10000000;
	PORTD &= 0b10000000;
	while (angle < 920){ //830
		
		
		cli();
		tempG = gyroData;
		sei();
		angle += fabs(tempG)*0.01;
		_delay_ms(100);
	}
	angle = 0;
	stop_servos();
	count_walls-=1;
	PORTD &= 0b00000000;
	
}

void rotate_90_left(void){
	rotate_left();
	PORTD |= 0b10000000;
	PORTD &= 0b10000000;
	while (angle < 920){ //830
		
		cli();
		tempG = gyroData;
		sei();
		angle += fabs(tempG)*0.01;
		_delay_ms(100);
	}
	
	angle = 0;
	stop_servos();
	count_walls +=1;
	PORTD &= 0b00000000;
	
} 
