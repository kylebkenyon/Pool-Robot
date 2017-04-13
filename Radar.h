#ifndef RADAR_H
#define RADAR_H

#include "NeatGUI.h"
#include "RadarPin.h"
#include "Base_Process.h"
#include <vector>
#include <map>
#include <string>
#include <functional>   // std::minus, std::divides
#include <numeric>      // std::inner_product
#include <iostream>

using namespace std;

class Radar : public Base_Process {
  private:
      int width;
      int height;
      int xCenter;
      int yCenter ;
      int xOffset;
      int yOffset;
      unsigned int colorForeground ;
      unsigned int colorBackground ;
      SSD1351_SPI * display;
      vector<int> x1;
      vector<int> y1;
      vector<int> circleSizes;
      map<string,RadarPin> pins;
      int i;
      int old_i;
      bool isLineVisible;

      const static int CIRCLE_BLINKING_DOT_RADIUS = 3;
      const static int NUMBER_OF_CIRCLES = 4;
      const static int FULL_SCREEN_WIDTH = 128;
      const static int FULL_SCREEN_HEIGHT = 128;
      // const static int PIN_VISIBILITY_COUNT = 1000;

      int refinement_sweep;

      void drawStaticRadar();
      void drawBlinkingDotRadar(RadarPin pin);
      void drawSweepingLine();
      void refresh();
      void decideIfPinHit();
      float crossproduct(int u_x, int u_y, int v_x, int v_y);
    public:
      Radar(SSD1351_SPI * displayIn, unsigned int colorForegroundIn, unsigned int colorBackgroundIn, int w, int h, int xOffsetIn, int yOffsetIn);
      void draw();
      void addRadarPin(RadarPin pin);
      virtual void callback();
      void output_pins(Serial * pcin);
};

#endif
