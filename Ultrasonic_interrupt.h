#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "NeatGUI.h"
#include "Base_Process.h"
#include "Radar.h"
#include "Motor.h"
#include <string>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define PI 3.14159265

class Ultrasonic : public Base_Process {
private:
  DigitalOut * trigger;
  InterruptIn * echo;
  Timer timer;
  Radar * radar;
  Timeout timeout;
  Motor *motorP;
  volatile int done;
  int distance;
  int start;
  int end;
  int tempDistance;
  void updateDistance();
  string intToString ( int number );
public:
  void interruptStart(void);
  void timerStart();
  virtual void callback();
  Ultrasonic(DigitalOut * triggerIn, InterruptIn * echoIn, Radar * radarIn, Motor * motorIn);
};

#endif
