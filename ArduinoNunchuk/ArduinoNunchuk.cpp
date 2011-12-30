/*
 * ArduinoNunchuk.cpp - Improved Wii Nunchuk library for Arduino
 * 
 * Copyright 2011-2012 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
 *
 * Based on the following projects/websites:
 *   http://www.windmeadow.com/node/42
 *   http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 * 
 */

#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Wire.h>
#include "ArduinoNunchuk.h"

#define ADDRESS 0x52

void ArduinoNunchuk::init()
{      
  Wire.begin();
  
  ArduinoNunchuk::_sendByte(0x55, 0xF0);
  ArduinoNunchuk::_sendByte(0x00, 0xFB); 
  
  ArduinoNunchuk::update();
}
    
void ArduinoNunchuk::update()
{ 
  int count = 0;      
  int values[5];
  
  Wire.requestFrom (ADDRESS, 6); 
  
  while(Wire.available())
  {
    values[count] = Wire.read();
    count++;
  } 
  
  ArduinoNunchuk::analogX = values[0];
  ArduinoNunchuk::analogY = values[1];      
  ArduinoNunchuk::accelX = values[2] * 2 * 2 + ((values[5] >> 2) & 1) * 2 + ((values[5] >> 3) & 1);
  ArduinoNunchuk::accelY = values[3] * 2 * 2 + ((values[5] >> 4) & 1) * 2 + ((values[5] >> 5) & 1);
  ArduinoNunchuk::accelZ = values[4] * 2 * 2 + ((values[5] >> 6) & 1) * 2 + ((values[5] >> 7) & 1);
  ArduinoNunchuk::zButton = !((values[5] >> 0) & 1);
  ArduinoNunchuk::cButton = !((values[5] >> 1) & 1);
  
  ArduinoNunchuk::_sendByte(0x00, 0x00);
}
  
void ArduinoNunchuk::_sendByte(byte data, byte location)
{  
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
