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
      delay(500);
    }
    Serial.print("\nGesture Sensor OK\n");
    I2C_writeByte(PAJ_BANK_SELECT, 0);//Select Bank 0
    for (i = 0; i < Gesture_Array_SIZE; i++)
    {
      I2C_writeByte(Init_Gesture_Array[i][0], Init_Gesture_Array[i][1]);//Gesture register initializes
    }
}

void loop() {
    Gesture_Data = I2C_readU16(PAJ_INT_FLAG1);
    if (Gesture_Data)
    {
      switch (Gesture_Data)
      {
        case PAJ_UP:                   Serial.print("Up\r\n");            break;
        case PAJ_DOWN:                 Serial.print("Down\r\n");          break;
        case PAJ_LEFT:                 Serial.print("Left\r\n");          break;
        case PAJ_RIGHT:                Serial.print("Right\r\n");         break;
        case PAJ_FORWARD:              Serial.print("Forward\r\n");       break;
        case PAJ_BACKWARD:             Serial.print("Backward\r\n");      break;
        case PAJ_CLOCKWISE:            Serial.print("Clockwise\r\n");     break;
        case PAJ_COUNT_CLOCKWISE:      Serial.print("AntiClockwise\r\n"); break;
        case PAJ_WAVE:                 Serial.print("Wave\r\n");          break;
        default: break;
      }
      Gesture_Data=0;
    }
}
