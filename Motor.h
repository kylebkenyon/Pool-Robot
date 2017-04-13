#ifndef MOTOR_H
#define MOTOR_H

#include "NeatGUI.h"
#include "Base_Process.h"
#include "mbed.h"
#include <string>
#include "Compass.h"
#include <cmath>

using namespace std;

class Motor : public Base_Process {
private:
  float step;
  const static int pattern[8][4];
  DigitalOut *a;
  DigitalOut *b;
  DigitalOut *c;
  DigitalOut *d;
  float max_count;
  bool isRunning;
  bool isFindingSpecifiedHeading;
  Compass *com;
  float specifiedHeading;


  int degrees_count;
  int degrees;
  static const float DEGREES_15 = 23.3;
  static const float DEGREES_180 = 280.0;
  static const float DEGREES_360 = 560.0;
  static const int HEADING_THRESHOLD = 5;

  bool isClockwise;
  Timer t;
  void delay(float del);
public:
  Motor(DigitalOut *a1, DigitalOut *b1, DigitalOut *c1, DigitalOut *d1, Compass *c);
  void motor_main();
  virtual void callback();
  void setDirection(bool isclock);
  int get_degrees();
  void stop();
  void start();
  void scanAtHeading(float target_degrees);

};
#endif
