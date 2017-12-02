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
signed char desired_value=0;
signed char actual_value;
signed char D;
signed char PID_value;
signed char error;
signed char strong1=30;
signed char strong2=-20;
signed char weak1=10;
signed char weak2=-5; // -10

volatile uint8_t temp0;
volatile uint8_t temp1;
volatile uint8_t temp2;
volatile uint8_t temp3;
volatile uint8_t temp4;
volatile uint8_t temp5;
volatile uint8_t temp6;


void rotate_right(void);
void rotate_left(void);
void pid_forward(void);
void backward(void);
void forward(void);





void terminal_view (uint8_t adc_channel1,uint8_t adc_channel2, uint8_t actual);

void PID(void);

int main(void)
{
	
	DDRB = 0xFF;
	DDRA =0x00;
	
	UART_init();
	UART_usb_init();
	sei();
	

	while(1)
	{	
		temp1 = PINA & 0b00000111;
		_delay_ms(10);
		if (temp1 == 0b00000111){
			sensor_distance=1; //ändra sen till att lagra vägg
		}
		
	
	
		if(sensor_distance == 0){
			forward();
		}else{
			PORTB &= 0b00000000;
		}
	
		
		
		
		

		
		
	}
	
	
}
ISR(USART0_RX_vect){
	data_received_usb= UDR0;
	
	
}
ISR(USART1_RX_vect){
	
	data_received_sensor = UDR1;
	//UART_usb_transmitByte(data_received_sensor);
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
	}

	
	


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
	
	if((sensor_right_front > 181) && (sensor_right_back < 181) ){
		
		UART_transmitByte((weak2+PID_value)); // lugnt
		
	}else if( (sensor_right_front > 181) && ( sensor_right_back > 181)){
		
		UART_transmitByte((strong2+PID_value)); //stark
		
	}else if((sensor_right_front < 152) && (sensor_right_back > 152)){ // 152
		
		UART_transmitByte((weak1+PID_value));  //lugn
		
	}else if((sensor_right_front < 152) && (sensor_right_back < 152)){
		
		UART_transmitByte((strong1+ PID_value)); //stark
		
	}else{
		UART_transmitByte(PID_value);
	}
		
	
}

void rotate_right(){
	
	PORTB |= (1 << PINB2);
	PORTB &= 0b00000100;
	_delay_ms(7750);
	PORTB &= 0b00000000;
	_delay_ms(10);
	
}

void rotate_left(){
	
	PORTB |= (1 << PINB3);
	PORTB &= 0b00001000;
	_delay_ms(7750);
	
	PORTB &= 0b00000000;
	_delay_ms(10);
}

void pid_forward(){
	

	PORTB |= 0b00001100;
	PORTB &= 0b00001100;
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
