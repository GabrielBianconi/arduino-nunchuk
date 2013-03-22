/*
 * ArduinoNunchuk.cpp - Improved Wii Nunchuk library for Arduino
 *
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
 *
 * Based on the following resources:
 *   http://www.windmeadow.com/node/42
 *   http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include "ArduinoNunchuk.h"

#define ADDRESS 0x52

void ArduinoNunchuk::init()
{
  Wire.begin();
  ArduinoNunchuk::reinit();

}
void ArduinoNunchuk::reinit()
{
  //init the nunchuk 
  ArduinoNunchuk::_sendByte(0x55, 0xF0);
  delay(10);
  ArduinoNunchuk::_sendByte(0x00, 0xFB);
  delay(10);
  //prime the nunchuk for reading data
  ArduinoNunchuk::_sendByte(0x00, 0x00);
  delay(10);
 
  ArduinoNunchuk::update();
  

}

void ArduinoNunchuk::update()
{
  uint8_t count = 0;      
  uint8_t values[6];
  uint8_t errors = 0;

  Wire.requestFrom(ADDRESS, 6);

  while(Wire.available() && count <= 6)
  {
    values[count] = Wire.read();
    if(values[count] == 0xFF || values[count] == 0x00) errors++;
    count++;
  }

  //detect receiving only partial data.
  if(count != 6) {
    ArduinoNunchuk::pluggedin = false;
    return;
  }

  //detect the nunchuk being unplugged and attempt to reconnect.
  
  if(errors >= 6) {
    ArduinoNunchuk::reinit();
    return;
  }
  
  ArduinoNunchuk::analogX = values[0];
  ArduinoNunchuk::analogY = values[1];
  ArduinoNunchuk::accelX = (values[2] << 2) | ((values[5] >> 2) & 3);
  ArduinoNunchuk::accelY = (values[3] << 2) | ((values[5] >> 4) & 3);
  ArduinoNunchuk::accelZ = (values[4] << 2) | ((values[5] >> 6) & 3);
  ArduinoNunchuk::zButton = !((values[5] >> 0) & 1);
  ArduinoNunchuk::cButton = !((values[5] >> 1) & 1);
 

  //prime the nunchuk for reading data
  ArduinoNunchuk::_sendByte(0x00, 0x00);

//this check can't go in the init section because some controllers do not get re-init on a hot plug (memorex)

if(ArduinoNunchuk::pluggedin == false){
ArduinoNunchuk::pluggedin = true;
  ArduinoNunchuk::analogXcenter = ArduinoNunchuk::analogX;
  ArduinoNunchuk::analogYcenter = ArduinoNunchuk::analogY;
}

  ArduinoNunchuk::analogMagnitude = sqrt(pow((ArduinoNunchuk::analogXcenter-ArduinoNunchuk::analogX),2)+pow((ArduinoNunchuk::analogYcenter-ArduinoNunchuk::analogY),2));
  ArduinoNunchuk::analogAngle = (atan2(ArduinoNunchuk::analogYcenter-ArduinoNunchuk::analogY,ArduinoNunchuk::analogXcenter-ArduinoNunchuk::analogX)*57.2957);

}

void ArduinoNunchuk::_sendByte(byte data, byte location)
{
  Wire.beginTransmission(ADDRESS);

  Wire.write(location);
  Wire.write(data);

  Wire.endTransmission();

}
