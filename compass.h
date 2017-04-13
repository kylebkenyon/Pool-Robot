#ifndef COMPASS_H
#define COMPASS_H

#include "NeatGUI.h"
#include "Base_Process.h"
#include <string>
#include "mbed.h"
#include <math.h>
#include <iostream>
#include "GlobalCounter.h"
//INCLUDE WHATEVER WE USED FOR OUR PUTC

using namespace std;

class Compass : public Base_Process {
  private:
      int xOffset;
      int yOffset;
      unsigned int foregroundColor;
    unsigned int backgroundColor;
      SSD1351_SPI * display;
      I2C * channel;
    string old_direction;
    float currHeading;

      float mx;
      float my;
      //float mz;

    const static int ADDRESS_MAGNETISM = 0x3c;
    const static int READ_LENGTH = 6;
    const static int CHANNEL_FREQUENCY = 400000;
    const static int EIGHT_BIT_SHIFT = 8;
    //const static float WAIT_TIME = 1.0;
    const static char CRA_REG_M   = 0x00;
    const static char CRB_REG_M   = 0x01;
    const static char MR_REG_M    = 0x02;
    const static char OUT_X_MAG   = 0x03;
    const static int GAIN_X = 1100;
    const static int GAIN_Y = 1100;
    //const static int GAIN_Z = 980;
    const static float Pi = 3.14159;

    bool write_reg(int addr_i2c,int addr_reg, char v);
    bool receive(char address, char outAddress, char *buffer);

    public:
        Compass(I2C * channelIn, SSD1351_SPI * displayIn, unsigned int foregroundColorIn, unsigned int backgroundColorIn, int xOffsetIn, int yOffsetIn);
        bool readMagnetismVector();
        float calc_heading(float mx, float my);
        string direction(float heading);
        void draw();
        virtual void callback();
      float getHeading();
};

#endif
