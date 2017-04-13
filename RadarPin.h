#ifndef RADARPIN_H
#define RADARPIN_H

#include <string>

using namespace std;

// always assumes the screen is 128x128

class RadarPin {
    public:
      string name;
      int x;
      int y;
      int x_normalized;
      int y_normalized;
      unsigned int color;
      // bool visibleDot;
      int visibility_left;
      bool hit;
      bool fresh;
      RadarPin(string n, int xIn, int yIn, unsigned int c);
      RadarPin(string n, int xIn, int yIn);

    friend bool operator<(const RadarPin &lhs, const RadarPin &rhs) {
        return (lhs.name < rhs.name);
    }

    friend bool operator==(const RadarPin &lhs, const RadarPin &rhs) {
        return (lhs.name == rhs.name);
    }

    friend bool operator!=(const RadarPin &lhs, const RadarPin &rhs) {
        return !(lhs.name == rhs.name);
    }
};

#endif
