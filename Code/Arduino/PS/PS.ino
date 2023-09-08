#include <Wire.h>
#include "PAJ7620U2.h"

unsigned short  Gesture_Data;
void setup() {
    uint8_t i;
    Serial.begin(115200);
    Serial.print("\nGesture Sensor Test Program ...\n");
    delayMicroseconds(800);
    Wire.begin();
    delayMicroseconds(10);
    while(!PAJ7620U2_init())
    { Serial.print("\nGesture Sensor Error\n");
      delay(200);
    }
    Serial.print("\nGesture Sensor OK\n");
    I2C_writeByte(PAJ_BANK_SELECT, 0);//Select Bank 0
    for (i = 0; i < PS_Array_SIZE; i++)
    {
      I2C_writeByte(Init_PS_Array[i][0], Init_PS_Array[i][1]);//Gesture register initializes
    }
    I2C_writeByte(PAJ_BANK_SELECT, 0);  
}
void loop() {
    uint8_t i, OBJ_BRIGHTNESS;
    unsigned short  OBJ_SIZE;
    OBJ_BRIGHTNESS  = I2C_readByte(PAJ_OBJ_BRIGHTNESS);
    OBJ_SIZE    = I2C_readU16(PAJ_OBJ_SIZE_L);
    Serial.print("Object brightness = ");
    Serial.print(OBJ_BRIGHTNESS);
    Serial.print(" ， ");
    Serial.print("Object size = ");
    Serial.print(OBJ_SIZE);
    Serial.print("\r\n");
    delay(300);
}
