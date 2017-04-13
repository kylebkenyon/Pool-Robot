#include "Radar.h"

Radar::Radar(SSD1351_SPI * displayIn, unsigned int colorForegroundIn, unsigned int colorBackgroundIn, int w, int h, int xOffsetIn, int yOffsetIn) {
  // init variables
  width = w;
  height = h;
  colorForeground = colorForegroundIn;
  colorBackground = colorBackgroundIn;
  xOffset = xOffsetIn;
  yOffset = yOffsetIn;
  display = displayIn;

  // calculated variables
  xCenter = w / 2;
  yCenter = h / 2;
  refinement_sweep = h / 6;
  i = 0;

  // bottom (left to right)
  for (int val = 0; val < width; val = val + refinement_sweep) {
    x1.push_back(val);
    y1.push_back(width);
  }

  // right (bottom to top)
  for (int val = height; val > 0; val = val - refinement_sweep) {
    x1.push_back(width);
    y1.push_back(val);
  }

  // top (right to left)
  for (int val = width; val > 0; val = val - refinement_sweep) {
    x1.push_back(val);
    y1.push_back(0);
  }

  // left (top to bottom)
  for (int val = 0; val < height; val = val + refinement_sweep) {
    x1.push_back(0);
    y1.push_back(val);
  }


  // circle sizes
  int circle_size = refinement_sweep;
  for (int val = 1; val <= NUMBER_OF_CIRCLES; val++) {
    circleSizes.push_back(circle_size);
    circle_size += 10;
  }

  printf("radar init");
  printf("number of circles %d w:%d h:%d center(%d,%d) offset(%d,%d) \r\n", circleSizes.size(), width, height, xCenter, yCenter, xOffset, yOffset);
}

void Radar::drawStaticRadar() {
  for (int v = 0; v < circleSizes.size(); v++) {
    display->drawCircle(xOffset + xCenter,yOffset + yCenter,circleSizes[v],colorForeground);
  }
}

void Radar::drawBlinkingDotRadar(RadarPin pin) {
    //if the pin is fresh and never been seen, don't draw
    if (pin.fresh) {
      return;
    }
    // display->fillCircle(xOffset + pin.x, yOffset + pin.y,CIRCLE_BLINKING_DOT_RADIUS, (pin.visibility_left > 0) ? colorForeground : colorBackground);
    display->fillCircle(xOffset + pin.x, yOffset + pin.y,CIRCLE_BLINKING_DOT_RADIUS, (pin.hit) ? colorForeground : colorBackground);
}

void Radar::drawSweepingLine() {
    display->drawLine(xOffset + xCenter, yOffset + yCenter, xOffset + x1[(isLineVisible) ?  i : old_i], yOffset + y1[(isLineVisible) ?  i : old_i], (isLineVisible) ? colorForeground : colorBackground);
}

void Radar::addRadarPin(RadarPin pin) {
  //only add if the pin doesn't already exist
  if (pins.find(pin.name) != pins.end()) {
    return;
  }

  // scaled to the size of the radar
  pin.x = pin.x * (width / FULL_SCREEN_WIDTH);
  pin.y = pin.y * (width / FULL_SCREEN_HEIGHT);

  //normalize for radar detection
  pin.x_normalized = pin.x - xCenter;
  pin.y_normalized = pin.y - yCenter;

  // printf("Adding: %s\r\n", pin.name);
  pins.insert ( std::pair<string,RadarPin>(pin.name,pin) );
}

void Radar::refresh() {
  map<string, RadarPin>::iterator it;
  for (it = pins.begin(); it != pins.end(); it++) {
      it->second.visibility_left = 0;
      it->second.hit = false;
      drawBlinkingDotRadar(it->second);
  }
  pins.clear();
}

float Radar::crossproduct(int u_x, int u_y, int v_x, int v_y) {
  return (u_x * v_y * 1.0) - (u_y * v_x * 1.0);
}

void Radar::decideIfPinHit() {
  //update pin visibility
  int old_x_normalized = x1[old_i] - xCenter;
  int old_y_normalized = y1[old_i] - yCenter;
  int x_normalized = x1[i] - xCenter;
  int y_normalized = y1[i] - yCenter;
  map<string, RadarPin>::iterator it;
  for (it = pins.begin(); it != pins.end(); it++) {
     // a old_line
     // b new_line
     // c pin
    // U x V = Ux*Vy-Uy*Vx
    // if (AxB * AxC >= 0 && CxB * CxA >= 0)
    float cross1 = crossproduct(old_x_normalized, old_y_normalized, x_normalized, y_normalized) * crossproduct(old_x_normalized, old_y_normalized, it->second.x_normalized, it->second.y_normalized) ;
    float cross2 = crossproduct(it->second.x_normalized, it->second.y_normalized, x_normalized, y_normalized) * crossproduct(it->second.x_normalized, it->second.y_normalized, old_x_normalized, old_y_normalized) ;

     if (cross1 >= 0 && cross2 >= 0) {
      //  printf("Hit for the first time %s, crosses (%f,%f)\r\n", it->first, cross1, cross2);
      //  printf("old (%d,%d) new (%d,%d) pin (%d,%d) cross %d\r\n", old_x_normalized, old_y_normalized, x_normalized, y_normalized, it->second.x_normalized, it->second.y_normalized, cross);
      //  it->second.visibility_left = PIN_VISIBILITY_COUNT;
       it->second.hit = true;
       it->second.fresh = false;
     }
    //  else if ( it->second.visibility_left > 0){
    //    it->second.visibility_left--;
    //  }

  }
}

void Radar::draw() {
  old_i = i;
  isLineVisible = true;
  i++;
  if (i >= x1.size()) {
    i = 0;
    refresh();
  } else {
    decideIfPinHit();
  }

  map<string, RadarPin>::iterator it;
  for (it = pins.begin(); it != pins.end(); it++) {
      drawBlinkingDotRadar(it->second);
  }
  drawStaticRadar();
  drawSweepingLine();
  isLineVisible = false;
  drawSweepingLine(); //remove old line

}

void Radar::callback() {
  draw();
}

void Radar::output_pins(Serial * pcin){
  map<string, RadarPin>::iterator it;
  for (it = pins.begin(); it != pins.end(); it++) {
      pcin->printf("Object at (x,y): (%d,%d)\r\n",  (xOffset + it->second.x) , (yOffset + it->second.y));
  }

}
