#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <math.h>
#include "PAJ7620U2.h"
int fd;
/******************* PAJ7620U2 Driver Interface *****************************/
char I2C_readByte(int reg)
{
	return wiringPiI2CReadReg8(fd, reg);
}

unsigned short I2C_readU16(int reg)
{
	return wiringPiI2CReadReg16(fd, reg);
}
void I2C_writeByte(int reg, int val)
{
	wiringPiI2CWriteReg8(fd, reg, val);
}
unsigned char PAJ7620U2_init()
{
	unsigned char i,State;
	fd=wiringPiI2CSetup(PAJ7620U2_I2C_ADDRESS);
	delay(5);
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
	unsigned char i, OBJ_BRIGHTNESS;
	unsigned short  OBJ_SIZE;
	printf("\nGesture Sensor Test Program ...\n");
	if (wiringPiSetup() < 0) return 1;
	delay(5);
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
	I2C_writeByte(PAJ_BANK_SELECT, 0);//Select Bank 0
	while (1)
	{
		OBJ_BRIGHTNESS = I2C_readByte(PAJ_OBJ_BRIGHTNESS);
		OBJ_SIZE = I2C_readU16(PAJ_OBJ_SIZE_L);
		printf("Object brightness = %d ,Object size = %d\n", OBJ_BRIGHTNESS, OBJ_SIZE);//Object Brightness (Max. 255),Object Size (Max. 900)
		delay(50);
	}
	return 0;
}
