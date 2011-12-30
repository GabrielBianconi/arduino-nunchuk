/*
 * ArduinoNunchuk.h - Improved Wii Nunchuk library for Arduino
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

#ifndef ArduinoNunchuk_H
#define ArduinoNunchuk_H

#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

class ArduinoNunchuk
{  
  public:  
    int analogX;
    int analogY;
    int accelX;
    int accelY;
    int accelZ;
    int zButton;
    int cButton;
  
    void init();    
    void update();
    
  private:  
    void _sendByte(byte data, byte location);
};

#endif
