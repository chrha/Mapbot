
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "UART_USB.h"
#include "UART_sensor.h"
#include <stdbool.h>

volatile unsigned char data_received_usb;
volatile unsigned char data_received_sensor;
signed char KP=6;
signed char KI=0;
signed char KD=5;
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

volatile uint8_t north=1;
volatile uint8_t south=2;
volatile uint8_t east=3;
volatile uint8_t west=4;

volatile uint8_t direction = 1;

volatile int8_t x=0;
volatile int8_t y=0;
volatile int8_t start_x=-1;
volatile int8_t start_y=-1;
uint8_t s=1;
void rotate_right(void);
void rotate_left(void);
void pid_forward(void);
void backward(void);
void forward(void);
void stop_servos(void);
void rotate_90_left(void);
void rotate_90_right(void);
void sendData(void);
void send_coordinates(void);
void automode(void);
void hard_stop(void);
double angle = 0;
double tempG;
double gyroData = 0;
char k=0;
void PID(void);
struct coordinates{
int x;
int y;
};

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
		automode();
	

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
		sensor_right_back=data_received_sensor+5;
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
		
		//count_walls +=1;
		if (direction == north){
			y += 1;
		}else if(direction == south){
			y -= 1;
		}else if(direction == east){
			x += 1;
		}else if(direction == west){
			x -= 1;
		}
		//send_coordinates();
	
		UART_usb_transmitByte('k');
		/*if((sensor_left >= 254) | (sensor_left <= 100)){
			UART_usb_transmitByte('o');
		}*/
	}
	
	if((x == start_x) && (y == start_y)){
		stop_servos();
		while(1){}
	}
	
	gyroData = (int16_t)(sensor_gyro_low | (int16_t)(sensor_gyro_high <<8));
	gyroData *= 0.07;
	
	
	
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
	//126
	if((sensor_right_front > 170) && (sensor_right_back < 170) ){
		
		UART_transmitByte((weak1+PID_value)); // lugnt
		
	}else if( (sensor_right_front > 170) && ( sensor_right_back > 170)){
		
		UART_transmitByte((strong1+PID_value)); //stark
		
	}/*if((sensor_right_front < 75) && (sensor_right_back > 75)){ 
		
		UART_transmitByte((weak2+PID_value));  //lugn
		
	}else if((sensor_right_front < 75) && (sensor_right_back < 75)){
		
		UART_transmitByte((strong2+ PID_value)); //stark
		
	}*/else{
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
	
	if(direction == north){
		direction= east;
		UART_usb_transmitByte('e');
		//y -= 1;
		
	}else if(direction == east){
		direction = south;
		UART_usb_transmitByte('s');
		//x -= 1;
		
	}else if(direction == south){
		direction = west;
		UART_usb_transmitByte('w');
		//y += 1;
		
	}else if (direction == west){
		direction = north;
		UART_usb_transmitByte('n');
		//x += 1;
		
	} 
	if(s){
		start_x=0;
		start_y=0;
		s=0;
	}
	//count_walls-=1;
	PORTD &= 0b00000000;
	//send_coordinates();
	_delay_ms(10);
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
	
	if(direction == north){
		direction= west;
		//x -= 1;
		UART_usb_transmitByte('w');
		_delay_ms(500);
		UART_usb_transmitByte('k'); 
		
	}else if(direction == west){
		direction = south;
		UART_usb_transmitByte('s');
		_delay_ms(500);
		UART_usb_transmitByte('k'); 
	
		//y -= 1;
	}else if(direction == south){
		direction = east;
		UART_usb_transmitByte('e');
		_delay_ms(500);
		UART_usb_transmitByte('k'); 
	
		//x += 1;
	}else if (direction == east){
		direction = north;
		UART_usb_transmitByte('n');
		_delay_ms(500);
		UART_usb_transmitByte('k'); 
	
		//y += 1;
	}
	
	//count_walls +=1;
	PORTD &= 0b00000000;
	//send_coordinates();
	_delay_ms(10);
}

void sendData(){
	UART_usb_transmitByte('r');
	_delay_ms(5);
	UART_usb_transmitByte(sensor_right_front);
	_delay_ms(500);
	UART_usb_transmitByte('b');
	_delay_ms(500);
	UART_usb_transmitByte(sensor_right_back);
	_delay_ms(500);
	UART_usb_transmitByte('f');
	_delay_ms(500);
	UART_usb_transmitByte(sensor_front);
	_delay_ms(500);
	UART_usb_transmitByte('l');
	_delay_ms(500);
	UART_usb_transmitByte(sensor_left);
	_delay_ms(500);
	UART_usb_transmitByte('d');
	_delay_ms(500);
	UART_usb_transmitByte(sensor_distance);
	_delay_ms(500);
}

void send_coordinates(){
	char mychar1[4];
	UART_usb_transmitByte('x');
	UART_usb_transmitByte(':');
	sprintf(mychar1, "%04d", x);
	if(x <0){
		UART_usb_transmitByte('-');
	}
	UART_usb_transmitByte(mychar1[1]);
	UART_usb_transmitByte(mychar1[2]);
	UART_usb_transmitByte(mychar1[3]);
	_delay_ms(10);

	
	char mychar2[4];
	UART_usb_transmitByte(' ');
	UART_usb_transmitByte('y');
	UART_usb_transmitByte(':');
	sprintf(mychar2, "%04d", y);
	if(y <0){
		UART_usb_transmitByte('-');
	}
	UART_usb_transmitByte(mychar2[1]);
	UART_usb_transmitByte(mychar2[2]);
	UART_usb_transmitByte(mychar2[3]);
	UART_usb_transmitByte(13);
	_delay_ms(10);
	
	
}
void automode(void){

	pid_forward();


	if (error >= 55){
	
		
		PORTD |= 0b10000000;
		PORTD &= 0b10000000;
		cli();
		forward();
		_delay_ms(5000);
		hard_stop();
		if(direction == north){
		
			//y -= 1;
			y+=1;
		}else if(direction == east){
		
			//x -= 1;
			x+=1;
		}else if(direction == south){
		
			//y += 1;
			y-=1;
		}else if (direction == west){
			//x += 1;
			x-=1;
		}
		
		PORTD &= 0b00000000;
		sei();
		_delay_ms(10000);
		rotate_90_right();
		_delay_ms(10000);
		stop_servos();
		_delay_ms(10000);
	//lägg till här
		forward();
		_delay_ms(10000);
		
	
	
		}else if ((error <=55) && (sensor_front >= 159) && (sensor_left >=120)){ //133
			hard_stop();
			_delay_ms(10000);
			rotate_90_left();
			stop_servos();
			_delay_ms(10000);
			rotate_90_left();
			stop_servos();
			_delay_ms(10000);
			while(sensor_left >= 181){
				if (error >= 55){
					PORTD |= 0b10000000;
					PORTD &= 0b10000000;
					cli();
					forward();
					_delay_ms(5000);
					hard_stop();
					if(direction == north){
						
						//y -= 1;
						y+=1;
						}else if(direction == east){
						
						//x -= 1;
						x+=1;
						}else if(direction == south){
						
						//y += 1;
						y-=1;
						}else if (direction == west){
						//x += 1;
						x-=1;
					}
					PORTD &= 0b00000000;
					sei();
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
	
	
		} else if ((error <=55) && (sensor_front >= 159)){
			hard_stop();
			_delay_ms(10000);
			rotate_90_left();
			stop_servos();
			_delay_ms(10000);
		}else{
			pid_forward();
	}

	// 43(ca 80 cm) stanna om ö hittad





}
void hard_stop(void){
	stop_servos();
	backward();
	_delay_ms(2250);
	stop_servos();
}
