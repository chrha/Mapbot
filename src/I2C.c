#include <avr/io.h>
#include "UART_sensor.h"
#define TWBRV ((F_CPU / 100000) - 16) / 2

void error_I2C(void);
void passed_I2C(void);
void I2C_StartOrRestart(void);
void I2C_CheckStatusReg(uint8_t Status);
void I2C_WaitForFlagToBeSet(void);
void I2C_ClearFlagAndEnable_WithACK(void);
void I2C_ClearFlagAndEnable(void);
void I2C_init(void);
uint8_t I2C_receive(uint8_t regAdr);


void error_I2C(void){
	UART_transmitByte('e');	
}
void passed_I2C(void){
	UART_transmitByte('p');
	

}
void I2C_WaitForFlagToBeSet(void){
	while ( !(TWCR & (1<<TWINT))){
		
	}
	
}
void I2C_CheckStatusReg(uint8_t Status){
	if ((TWSR & 0xF8) != Status){ //check that its actualy a start cond
		error_I2C();}
	else {
		passed_I2C();
		
	}
}
void I2C_ClearFlagAndEnable(void){
	TWCR = (1 << TWINT) | (1<<TWEN);
}
void I2C_StartOrRestart(void){
	TWCR = (1<<TWEN) | (1<<TWINT) | (1 << TWSTA);
}
void I2C_ClearFlagAndEnable_WithACK(void){
	TWCR = (1 << TWINT) | (1<<TWEN) | (1<<TWEA);
}
void I2C_init(void){
	PRR0 &= ~(1<<PRTWI); // turn off power reduction so I2C is nod disabled
	TWCR &= ~(1<<TWIE); //turn off interrupt
	TWBR = TWBRV; //set bitrate
	TWSR &= ~(1<<TWPS1)| (0<<TWPS0); //No prescaler to mess up the status reg
}
uint8_t I2C_receive(uint8_t regAdr){
	//start cond
	TWCR =0;
	I2C_StartOrRestart();
	I2C_WaitForFlagToBeSet(); //wait for flag to be set
	I2C_CheckStatusReg(0x08);
	
	
	//send slave addr and w/r
	TWDR = 0xD6; // +1 om R +0 om W
	I2C_ClearFlagAndEnable(); // CLEAR THE FLAG AND ENABLE
	I2C_WaitForFlagToBeSet();
	I2C_CheckStatusReg(0x18); // Check status for SLA + W transminted and ACK Rcv'd
	
	// send register address
	TWDR = regAdr;
	I2C_ClearFlagAndEnable(); // CLEAR THE FLAG AND ENABLE
	I2C_WaitForFlagToBeSet();
	I2C_CheckStatusReg(0x28); // Check status for SLA + W transminted and ACK Rcv'd
	
	//restart
	I2C_StartOrRestart();
	I2C_WaitForFlagToBeSet(); //wait for flag to be set
	I2C_CheckStatusReg(0x10);
	
	//send slave addr and w/r
	TWDR = 0xD7; // +1 om R +0 om W
	I2C_ClearFlagAndEnable(); // CLEAR THE FLAG AND ENABLE
	I2C_WaitForFlagToBeSet();
	I2C_CheckStatusReg(0x40); // Check status for SLA + W transminted and ACK Rcv'd
	
	
	
	I2C_ClearFlagAndEnable(); // CLEAR THE FLAG AND ENABLE
	I2C_WaitForFlagToBeSet();
	I2C_CheckStatusReg(0x58); // Check status for SLA + W transminted and ACK Rcv'd
	
	PORTD = 0;
	PORTD = 0x80;
	PORTD = 0;
	
	//Read data
	uint8_t gyroData = 0;
	gyroData = TWDR;
	//I2C_ClearFlagAndEnable();
	//I2C_WaitForFlagToBeSet();
	//I2C_CheckStatusReg(0x58); // Check for dtata rcvd and nack sent
	
	
	
	
	// Stop Cond
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	return gyroData;
	
}

