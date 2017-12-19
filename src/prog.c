
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
void turn_left(void);
void turn_right(void);
signed char pid=0;
uint8_t temp0;
uint8_t temp1;
uint8_t temp2;
uint8_t temp3;
uint8_t temp4;
const unsigned char const_speed = 80;
int main(void)
{
	DDRA =0x00;
	DDRD=0xf3;
	PORTD &= 0b00000000;
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
	//pwm_drive();
	OCR3B = 80; // left
	OCR3A = 80; // right
}
void rotate_left()
{
	PORTD &= ~(1 << PORTD0); // Pin PD0 goes low (forward_right)
	PORTD |= (1 << PORTD1); // Pin PD1 goes low (forward_left)
	//pwm_drive();
	OCR3B = 80; // left
	OCR3A = 80; // right
}

void turn_left(){
	
	PORTD |= (1 << PORTD1); // Pin PD0 goes high (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes high (forward_left)
	OCR3B = 100; // left
	OCR3A = 30; // right
}

void turn_right(){
	
	PORTD |= (1 << PORTD1); // Pin PD0 goes high (forward_right)
	PORTD |= (1 << PORTD0); // Pin PD1 goes high (forward_left)

	OCR3B = 30; // left
	OCR3A = 100; // right
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
	
	}else if(temp0 == 6){
		turn_right();
	}else if(temp0 == 7){
		turn_left();
	}else{
		OCR3B = 0; 
		OCR3A = 0;
	}
} 


/*
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
#include "cordlist.h"
#include <avr/wdt.h>


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

signed char strong1=18;
signed char strong2=-20;
signed char weak1=5;
signed char weak2=-5; // -10

volatile uint8_t temp0;
volatile uint8_t temp1;
volatile uint8_t count_walls=1;

volatile uint8_t north=1;
volatile uint8_t south=2;
volatile uint8_t east=3;
volatile uint8_t west=4;

volatile uint8_t direction;

volatile struct coordinate current_pos;
volatile struct coordinate start_pos;

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
void send_coordinates(int8_t x,int8_t y);
void automode(void);
void hard_stop(void);
void discover_island(void);
void add_island(void);
void turn_right(void);
void turn_left(void);
double angle = 0;
double tempG;
double gyroData = 0;
char k=0;
struct coordinate Q;
struct coordinate K;

void PID(void);
int8_t one = 1;
int8_t find_island = 1;
int8_t check_island=0;
uint8_t count_start_position=0;
uint8_t enable_find_island=0;
struct coordinate end_of_island;
volatile char mode = 'p';
volatile uint8_t switch_mode;
int main(void)
{
	current_pos.pos_x=0;
	current_pos.pos_y=0;
	start_pos.pos_x=-1;
	start_pos.pos_y=-1;
	end_of_island.pos_x=-2;
	end_of_island.pos_y=-2;
	direction=north;
	switch_mode= PINA & 0b00100000;
	Q.pos_x=0;
	Q.pos_y=0;
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
		switch_mode= PINA & 0b00100000;
		if(switch_mode == 0b00100000){
			
		}else if(switch_mode == 0b00000000){
			discover_island();
			automode();
		}
		
		
		
	

	}
	
	
}
ISR(USART0_RX_vect){
	
	data_received_usb= UDR0;
	
	if (switch_mode == 0b00100000){
		if(data_received_usb == 'w'){
			forward();
		}else if(data_received_usb == 'a'){
			turn_left();
		}else if(data_received_usb == 's'){
			backward();
		}else if(data_received_usb == 'd'){
			turn_right();
		}else if(data_received_usb == '0'){
			stop_servos();
		}


	}

	
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
			//y += 1;
			current_pos.pos_y += 1;
		}else if(direction == south){
			//y -= 1;
			current_pos.pos_y -= 1;
		}else if(direction == east){
			//x += 1;
			current_pos.pos_x += 1;
		}else if(direction == west){
			//x -= 1;
			current_pos.pos_x -= 1;
		}

		UART_usb_transmitByte('k');
		
		


		add_island();
		

		if((current_pos.pos_x == start_pos.pos_x) && (current_pos.pos_y == start_pos.pos_y)){
			find_island=0;
			check_island=1;
			start_pos.pos_x=-1;
			start_pos.pos_y=-1;
			s=1;
			UART_usb_transmitByte('t');
			count_start_position +=1;
			if(count_start_position==2){
				stop_servos();
				while(1){}
			}
		}


	
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
	PID_value= (int8_t)((KP*P + KD* D + KI*I) * 0.53);
	
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
		
	}else{ // kommentera bort här
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


void turn_right(){

	PORTB |= 0b00011000;
	PORTB &= 0b00011000;
	_delay_ms(10);
}

void turn_left(){
	PORTB |= 0b00011100;
	PORTB &= 0b00011100;
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
	stop_servos();
	angle = 0;
	
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
		start_pos.pos_x=0;
		start_pos.pos_y=0;
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
	while (angle < 970){ //920
		
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

void add_island(void){
	if (find_island){
		if(rmv_position(current_pos)){  //rmv_position(current_pos);
			if((sensor_left >= 100) && (sensor_left <= 225)){
				UART_usb_transmitByte('o');
				Q.pos_x=current_pos.pos_x;
				Q.pos_y=current_pos.pos_y;
				insert_position(Q,Q);
				
				}else if((sensor_left >= 55) && (sensor_left <= 92)){
				UART_usb_transmitByte('z');
				K.pos_x=current_pos.pos_x;
				K.pos_y=current_pos.pos_y;
				if (direction == north){
					Q.pos_x=(current_pos.pos_x - 1);
					Q.pos_y=current_pos.pos_y;
					insert_position(Q,K);
					}else if(direction == south){
					Q.pos_x=(current_pos.pos_x + 1);
					Q.pos_y=current_pos.pos_y;
					insert_position(Q,K);
					}else if(direction == east){
					Q.pos_x=current_pos.pos_x;
					Q.pos_y=(current_pos.pos_y + 1);
					insert_position(Q,K);
					}else if(direction == west){
					Q.pos_x=current_pos.pos_x;
					Q.pos_y=(current_pos.pos_y - 1);
					insert_position(Q,K);
				}
			}
		}

	}
	

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

void send_coordinates(int8_t x,int8_t y){
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

	if (error >= 60){
	
		PORTD |= 0b10000000;
		PORTD &= 0b10000000;
		cli();
		forward();
		_delay_ms(10000);
		hard_stop();
		if(direction == north){
		
			current_pos.pos_y +=1;
			//y+=1;
		}else if(direction == east){
		
			current_pos.pos_x +=1;
			//x+=1;
		}else if(direction == south){
		
			current_pos.pos_y -=1;
			//y-=1;
		}else if (direction == west){
			current_pos.pos_x -=1;
			//x-=1;
		}
		
		PORTD &= 0b00000000;
		sei();
		_delay_ms(2000);
		rotate_90_right();
		_delay_ms(2000);
		if(enable_find_island){
			end_of_island.pos_x=x_values[0];
			end_of_island.pos_y=y_values[0];
			enable_find_island=0;
		}
	//lägg till här
		forward();
		_delay_ms(14000);
		}else if ((error <=60) && (sensor_front >= 225) && (sensor_left >=120)){ //133 front=161
			
			hard_stop();
			_delay_ms(1000);
			rotate_90_left();
			_delay_ms(1000);
			forward();
			while(sensor_front <=200){
				
			}
			stop_servos();
			_delay_ms(1000);
			rotate_90_left();
			add_island();
			_delay_ms(1000);
			while(sensor_left >= 181){
				if (error >= 60){
					PORTD |= 0b10000000;
					PORTD &= 0b10000000;
					cli();
					forward();
					_delay_ms(10000);
					hard_stop();
					if(direction == north){
						
						current_pos.pos_y +=1;
						//y+=1;
					}else if(direction == east){
						
						current_pos.pos_x +=1;
						//x+=1;
					}else if(direction == south){
						
						current_pos.pos_y -=1;
						//y-=1;
					}else if (direction == west){
						current_pos.pos_x -=1;
						//x-=1;
					}
					PORTD &= 0b00000000;
					sei();
					_delay_ms(2000);
					rotate_90_right();
					_delay_ms(2000);

					forward();
					_delay_ms(14000);
			
				}else{
					pid_forward();
				}
		
			}
	
	
		} else if ((error <=60) && (sensor_front >= 225)){
			
			hard_stop();
			_delay_ms(2000);
			rotate_90_left();
			_delay_ms(2000);
		}

		
		pid_forward();	
}



void hard_stop(void){
	stop_servos();
	backward();
	_delay_ms(2000);
	stop_servos();
}


void discover_island(){
	if(check_island){

		
		
		if((current_pos.pos_x == x_curr_values[0]) && (current_pos.pos_y == y_curr_values[0])){
			enable_find_island=1;
			stop_servos();
			_delay_ms(2000);
			rotate_90_left();
			_delay_ms(2000);
			forward();
			while(sensor_front <= 210){}
			stop_servos();
			_delay_ms(2000);
			rotate_90_left();
			_delay_ms(2000);
			UART_usb_transmitByte('t');
			check_island=0;
		
		}
	}

	if((current_pos.pos_x==end_of_island.pos_x) && (current_pos.pos_y==end_of_island.pos_y)){
		stop_servos();
		_delay_ms(2000);
		UART_usb_transmitByte('t');
		rotate_90_left();
		_delay_ms(2000);
		forward();
		while(sensor_front <= 210){}
		stop_servos();
		_delay_ms(2000);
		rotate_90_left();
		_delay_ms(2000);
		end_of_island.pos_x=-100;
		end_of_island.pos_y=-100;
		
	}
}



/*
#include <avr/io.h>
#define F_CPU 14745600

#include <util/delay.h>
#include "UART_sensor.h"
#include "I2C.h"
#include <stdio.h>
#include <string.h>
void adc_init();
uint8_t adc_read(uint8_t ch);
void SendString(char mydata[20]);
void terminal_view (uint8_t adc_channel1, uint8_t adc_channel2);
uint8_t temp0, temp1;
void what_to_send(void);
uint16_t s1=0;
uint16_t s2=0;
signed char actual_value=0;
void sensor_right_front(void);
void sensor_right_back(void);
void sensor_front(void);
void sensor_left(void);
void sensor_gyro(void);
void sensor_distance(void);

uint8_t lenght_right();
uint8_t lenght_left();
uint8_t black_4=0;
uint8_t white_4=0;
uint8_t black_5=0;
uint8_t white_5=0;
uint8_t PB_4=0;
uint8_t PB_5=0;
uint8_t latest_color1=0;
uint8_t latest_color2=0;
float average_distance=0;
float left_distance=0;
float right_distance=0;
uint8_t average_front_distance=0;
uint8_t temp=0;
uint8_t buffer_index=0;
uint8_t buffer[10] = {0,0,0,0,0,0,0,0,0,0};
uint8_t average_left_value;
int tmp;
int main(void)
{
	DDRB = 0b01001111;
	DDRD = 0x00;
	PORTB &= 0b00000000;
	adc_init();
	UART_init();
	I2C_init();
	I2C_power();
	
	while(1)
	{
		//s1= (uint16_t)(floor(10*(27.6649/pow((adc_read(1)*2.5/255),(1.182033096926714)))));
		//s2= (uint16_t)(floor(10*(27.6649/pow((adc_read(0)*2.5/255),(1.182033096926714)))));
		//actual_value= (signed char)((uint8_t)(s1-s2));
		
		// notera att PINB4 inte funkar.enbart jordning.
		
		
		temp = PIND & 0b10000000;
		if(temp == 0b10000000)
		{
			black_4=0;
			black_5=0;
			white_4=0;
			white_5=0;
		}
		
		sensor_distance();
		
		sensor_right_front();
		sensor_right_back();
		
		sensor_distance();
		
		sensor_front();
		sensor_left();
		
		sensor_distance();
		
		sensor_gyro();	
		
		
	}
}
void adc_init()
{
	ADMUX|=0b11100000;
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
uint8_t adc_read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	
	
	while(ADCSRA & (1<<ADSC));
	
	return ADCH;
}
void SendString(char mydata[20])
{
	int i;
	for(i=0;i<strlen(mydata);i++)
	{
		UART_transmitByte(mydata[i]);
		
	}
}

void terminal_view (uint8_t adc_channel1, uint8_t adc_channel2)
{
	adc_channel1&=0b00000111;
	adc_channel2&=0b00000111;
	char mychar1[5];
	char mychar2[5];
	/*
	SendString("ADC_value1: ");
	sprintf(mychar1, "%04d", adc_read(adc_channel1));
	UART_transmitByte(mychar1[1]);
	UART_transmitByte(mychar1[2]);
	UART_transmitByte(mychar1[3]);
	UART_transmitByte(mychar1[4]);
	_delay_ms(50);
	
	SendString("   ADC_value2: ");
	sprintf(mychar2, "%04d", average_left_value);
	UART_transmitByte(mychar2[1]);
	UART_transmitByte(mychar2[2]);
	UART_transmitByte(mychar2[3]);
	UART_transmitByte(mychar2[4]);
	UART_transmitByte(13);
	_delay_ms(50);
	
	
	
} 


void sensor_right_front(){
	PORTB |= (1 << PINB0);
	PORTB &= 0b00000001;
	UART_transmitByte(adc_read(1));
	_delay_ms(5);
}

void sensor_right_back(){
	PORTB |= (1 << PINB1);
	PORTB &= 0b00000010;
	UART_transmitByte(adc_read(0));
	_delay_ms(5);
}

void sensor_front(){
	average_front_distance= ( adc_read(3)+adc_read(4) ) / 2;
	PORTB |= 0b00000011;
	PORTB &= 0b00000011;
	UART_transmitByte(average_front_distance);
	_delay_ms(5);
}


void sensor_left(){
	buffer[buffer_index]= adc_read(2);
	buffer_index++;
	if(buffer_index==10) buffer_index=0;
	for(int i=0; i<10; i++){
		tmp+=buffer[i];
	}
	average_left_value = (uint8_t)(tmp/10);
	tmp = 0;
	PORTB |= (1 << PINB2);
	PORTB &= 0b00000100;	
	UART_transmitByte(average_left_value);
	_delay_ms(5);
}

void sensor_gyro(){
	
	PORTB |= 0b00000101;
	PORTB &= 0b00000101;
	UART_transmitByte(I2C_receive(0x2C)); // low
	_delay_ms(5);
	
	PORTB |= 0b00000110;
	PORTB &= 0b00000110;
	UART_transmitByte(I2C_receive(0x2D)); // high
	_delay_ms(5);
}

void sensor_distance(void){
	left_distance=(float)lenght_left();
	right_distance= (float)lenght_right();
	average_distance = (left_distance + right_distance) / 2;
	
	
	if ( average_distance >= 30){ //ha 24. 29.5 på prick. 30 stor.
		
		PORTB |= 0b00000111;
		PORTB &= 0b00000111;
		UART_transmitByte('1');
		black_4=0;
		black_5=0;
		white_4=0;
		white_5=0;
	}
	else{
		PORTB = 0b00000000;
	}
	_delay_ms(5);
}  
 
uint8_t lenght_left()
{
	PB_4 = PINB & 0b00010000;
	
	
	// black = low, white =high
	if ((PB_4 == 0b00010000) && !(latest_color1==0b00010000))
	{
		white_4++;
		latest_color1 = PB_4;
		
	}
	else if ((PB_4 == 0b00000000) && !(latest_color1 == 0b00000000))
	{
		black_4++;
		latest_color1=PB_4;
	}
	
	return black_4 + white_4;
}


uint8_t lenght_right()
{
	PB_5 = PINB & 0b00100000;
	
	
	// black = low, white =high
	if ((PB_5 == 0b00100000) && !(latest_color2==0b00100000))
	{
		white_5++;
		latest_color2 = PB_5;
		
	}
	else if ((PB_5 == 0b00000000) && !(latest_color2 == 0b00000000))
	{
		black_5++;
		latest_color2=PB_5;
	}
	
	return black_5 + white_5;
}


*/

