#include <bcm2835.h>
#include <stdio.h>
#include <math.h>
#include "PAJ7620U2.h"
/******************* PAJ7620U2 Driver Interface *****************************/
char I2C_readByte(char reg)
{
	char buf[] = { reg };
	bcm2835_i2c_read_register_rs(buf, buf, 1);
	return buf[0];
}
unsigned short I2C_readU16(char reg)
{
	char buf[] = { reg,0 };
	bcm2835_i2c_read_register_rs(buf, buf, 2);
	int value = buf[1] * 0x100 + buf[0];
	return value;
}
void I2C_writeByte(char reg, char val)
{
	char buf[] = { reg,val };
	bcm2835_i2c_write(buf, 2);
}
uint8_t PAJ7620U2_init()
{
	uint8_t i,State;
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(PAJ7620U2_I2C_ADDRESS);
	bcm2835_i2c_set_baudrate(10000);
	bcm2835_delay(5);
	State = I2C_readByte(0x00);												//Read State
	if (State != 0x20) return 0;											//Wake up failed
	I2C_writeByte(PAJ_BANK_SELECT, 0);										//Select Bank 0
	for (i=0;i< Init_Array;i++)
	{
		I2C_writeByte(Init_Register_Array[i][0], Init_Register_Array[i][1]);//Power up initialize
	}
	return 1;
}

int main(int argc, char** argv)
{
	uint8_t i, OBJ_BRIGHTNESS;
	unsigned short  OBJ_SIZE;
	printf("\nGesture Sensor Test Program ...\n");
	if (!bcm2835_init()) return 1;
	bcm2835_delay(5);
	if(!PAJ7620U2_init())
	{	printf("\nGesture Sensor Error\n");
		return 0;
	}
	printf("\nGesture Sensor OK\n");
	I2C_writeByte(PAJ_BANK_SELECT, 0);//Select Bank 0
	for (i = 0; i < PS_Array_SIZE; i++)
	{
		I2C_writeByte(Init_PS_Array[i][0], Init_PS_Array[i][1]);//Gesture register initializes
	}
	I2C_writeByte(PAJ_BANK_SELECT, 0);	
	while (1)
	{
		OBJ_BRIGHTNESS	=	I2C_readByte(PAJ_OBJ_BRIGHTNESS);
		OBJ_SIZE		=	I2C_readU16(PAJ_OBJ_SIZE_L);
		printf("Object brightness = %d ,Object size = %d\n", OBJ_BRIGHTNESS, OBJ_SIZE);//Object Brightness (Max. 255),Object Size (Max. 900)
		delay(50);
	}
	return 0;
}
