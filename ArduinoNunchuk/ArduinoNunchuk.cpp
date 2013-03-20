
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
  init2();
}
void ArduinoNunchuk::init2()
{
  ArduinoNunchuk::pluggedin = false;
  ArduinoNunchuk::_sendByte(0x55, 0xF0);
  delay(10);
  ArduinoNunchuk::_sendByte(0x00, 0xFB);
  delay(10);
  ArduinoNunchuk::update();
  ArduinoNunchuk::analogXcenter = ArduinoNunchuk::analogX;
  ArduinoNunchuk::analogYcenter = ArduinoNunchuk::analogX;
  ArduinoNunchuk::pluggedin = true;
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
    if(values[count] == 0xff) errors++;
    count++;
  }

  //Detect getting only partial data.
  if(count != 6) {
    return;
  }

  //Detect unplugged nunchuck and attempt reconnect.
  if(errors == 6) {
    init2();
  }

  ArduinoNunchuk::analogX = values[0];
  ArduinoNunchuk::analogY = values[1];
  ArduinoNunchuk::accelX = (values[2] << 2) | ((values[5] >> 2) & 3);
  ArduinoNunchuk::accelY = (values[3] << 2) | ((values[5] >> 4) & 3);
  ArduinoNunchuk::accelZ = (values[4] << 2) | ((values[5] >> 6) & 3);
  ArduinoNunchuk::zButton = !((values[5] >> 0) & 1);
  ArduinoNunchuk::cButton = !((values[5] >> 1) & 1);

  ArduinoNunchuk::_sendByte(0x00, 0x00);
}

void ArduinoNunchuk::_sendByte(byte data, byte location)
{
  Wire.beginTransmission(ADDRESS);

  Wire.write(location);
  Wire.write(data);

  Wire.endTransmission();

}
