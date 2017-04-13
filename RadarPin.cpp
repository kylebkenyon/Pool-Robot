#include "RadarPin.h"

RadarPin::RadarPin(string n, int xIn, int yIn, unsigned int c) {
  name = n;
  x = xIn;
  y = yIn;
  color = c;
  visibility_left = 0; //default off
  hit = false;
  fresh = true;
}


RadarPin::RadarPin(string n, int xIn, int yIn) {
  name = n;
  x = xIn;
  y = yIn;
  // color = c;
  visibility_left = 0; //default off
  hit = false;
  fresh = true;
}
