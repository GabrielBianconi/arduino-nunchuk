/*
 * ArduinoNunchuk Library - Improved Wii Nunchuk library for Arduino
 * 
 * Copyright 2011 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
 *
 * Based on the following projects/websites:
 *   http://www.windmeadow.com/node/42
 *   http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 * 
 */


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
      
      int count = 0;      
      int values[5];
      
      Wire.requestFrom (ADDRESS, 6); 
      
      while(Wire.available()) {
        values[count] = Wire.read();
        count++;
      } 
      
      analogX = values[0];
      analogY = values[1];      
      accelX = values[2] * 2 * 2 + ((values[5] >> 2) & 1) * 2 + ((values[5] >> 3) & 1);
      accelY = values[3] * 2 * 2 + ((values[5] >> 4) & 1) * 2 + ((values[5] >> 5) & 1);
      accelZ = values[4] * 2 * 2 + ((values[5] >> 6) & 1) * 2 + ((values[5] >> 7) & 1);
      zButton = !((values[5] >> 0) & 1);
      cButton = !((values[5] >> 1) & 1);
      
      _sendByte(0x00, 0x00);
    }
    
  private:
  
    void _sendByte(byte data, byte location) {  
      
      Wire.beginTransmission(ADDRESS);
      
      #if (ARDUINO >= 100)
        Wire.write(location);
        Wire.write(data);  
      #else
        Wire.send(location);
        Wire.send(data); 
      #endif
      
      Wire.endTransmission();
      
      delay(10);
    }
    
};

#endif
