#ifndef ArduinoNunchuk_H
#define ArduinoNunchuk_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Wire.h>

#define ADDRESS 0x52

class ArduinoNunchuk {
  public:
  
    int analogX;
    int analogY;
    int accelX;
    int accelY;
    int accelZ;
    int zButton;
    int cButton;
  
    void init() {
      Wire.begin();
      _sendByte(0x55, 0xF0);
      _sendByte(0x00, 0xFB); 
      update();     
    }
    
    void update() {
      Wire.requestFrom (ADDRESS, 6);
      
      int count = 0;
      
      int values[5];
      
      while(Wire.available()) {
        values[count] = Wire.read();
        count++;
      } 
      
      analogX = values[0];
      analogY = values[1];
      accelX = values[2];
      accelY = values[3];
      accelZ = values[4];
      zButton = !((values[5] >> 0) & 1);
      cButton = !((values[5] >> 1) & 1);
      
      _sendByte(0x00, 0x00);
      delay(100);   
    }
    
  private:
    void _sendByte(byte data, byte location) {        
      Wire.beginTransmission(ADDRESS);
      #if (ARDUINO >= 100)
      Wire.write((uint8_t) location);
      Wire.write((uint8_t) data);  
      #else
      Wire.send((uint8_t) location);
      Wire.send((uint8_t) data); 
      #endif
      Wire.endTransmission();
      delay(10);
    }
    
};

#endif
