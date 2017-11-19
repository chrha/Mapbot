#include <avr/io.h>
void error_I2C(void)
void error_I2C(void){

}
void passed_I2C(void){

}
void I2CWaitForFlagToBeSet(){
while ( !(TWCR & (1<<TWINT))); 
}
void I2CCheckStatusReg(uint 8_t Status){
	if ((TWSR & Status) != Status){ //check that its actualy a start cond
		error_I2C();}
	else {
		passed_I2C();
		
	}
}
void I2CClearFlagAndEnable(){
TWCR = (1 << TWINT) | (1<<TWEN);
}
void I2C_StartOrRestart(){
	TWCR = (1<<TWEN) | (1<<TWINT) | (1 << TWSTA);
}
void I2CClearFlagAndEnable_WithACK(){
TWCR = (1 << TWINT) | (1<<TWEN) | (1<<TWEA);
}
void init_I2C(){
	PRR0 &= (0<<PRTWI); // turn off power reduction so I2C is nod disabled
	TWCR &= (0<<TWIE); //turn off interrupt
	TWBR = "beräkna" //set bitrate
	TWSR & = (0<<TWPS1)| (1<<TWPSO); //No prescaler to mess up the status reg
}
int main(void){
	
	
	// Init I2C
	init_I2C();
	
	//start cond
	TWCR = (1<<TWEN) | (1<<TWINT) | (1 << TWSTA);
	I2CWaitForFlagToBeSet(); //wait for flag to be set
	I2CCheckStatusReg(0x08);
	
	//send slave addr and w/r
	TWDR = slave_addr // +1 om R +0 om W
	I2CClearFlagAndEnable(); // CLEAR THE FLAG AND ENABLE
	I2CWaitForFlagToBeSet();
	I2CCheckStatusReg(0x18); // Check status for SLA + W transminted and ACK Rcv'd
	
	
	
	//write
	
	I2CClearFlagAndEnable();
	I2CWaitForFlagToBeSet();
	I2CCheckStatusReg(0x28);
	
	
	//repeat start 
	I2C_StartOrRestart();
	I2CWaitForFlagToBeSet();
	I2CCheckStatusReg(0x10);	
	
	//Send Slave addr and R
	TWDR = slave_addr + 1; // +1 om R +0 om W
	I2CClearFlagAndEnable(); // CLEAR THE FLAG AND ENABLE
	I2CWaitForFlagToBeSet();
	I2CCheckStatusReg(0x40); // Check status for SLA + R transminted and ACK Rcv'd
	
	 //Read data
	 uint8_t IDFromADXL = 0;
	 IDFromADXL = TWDR;
	 I2CClearFlagAndEnable();
	I2CWaitForFlagToBeSet();
	I2CCheckStatusReg(0x58); // Check for dtata rcvd and nack sent

	// Stop Cond
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	
	while(1){
	
	}
}