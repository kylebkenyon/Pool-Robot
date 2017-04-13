#include "Motor.h"

const int Motor::pattern[8][4] = {{0,0,0,1},{0,0,1,1},{0,0,1,0},{0,1,1,0},{0,1,0,0},{1,1,0,0},{1,0,0,0},{1,0,0,1}};

Motor::Motor(DigitalOut *a1, DigitalOut *b1, DigitalOut *c1, DigitalOut *d1, Compass *compassIn){

  a = a1;
  b = b1;
  c = c1;
  d = d1;
  degrees_count = 0;
  degrees = 0;
  step = 1;
  isClockwise = true;
  max_count = DEGREES_180;
  isRunning = true;
  com = compassIn;
}

void Motor::delay(float del)
{

    t.start();
    int st=t.read_ms();
    while(t.read_ms()-st!=del)
    {    }
    t.stop();

}

int Motor::get_degrees(){
  float out_of_degrees = 0.0;
  if (max_count == DEGREES_15)
    out_of_degrees = 15.0;
  else if (max_count == DEGREES_180)
    out_of_degrees = 180.0;
  else if (max_count == DEGREES_360)
    out_of_degrees = 360.0;

  if(isClockwise)
    return degrees_count * (out_of_degrees/max_count);
  else
    return (max_count - degrees_count) * (out_of_degrees/max_count);

}

void Motor::motor_main(){
    if(isClockwise){
       for(int i=0;i<8;i++)
           {
                *a=pattern[i][0];
                *b=pattern[i][1];
                *c=pattern[i][2];
                *d=pattern[i][3];
                delay(step);
            }
    }
    if(!isClockwise){
        for(int i=0;i<8;i++)
        {
                *a=pattern[8-i][0];
                *b=pattern[8-i][1];
                *c=pattern[8-i][2];
                *d=pattern[8-i][3];
                delay(step);
        }
    }
    degrees_count++;
}



void Motor::callback(){
  if(isRunning){
    if(degrees_count > max_count)
    {
      degrees_count = 0;
      isClockwise = !isClockwise;
      motor_main();
    }
    else
      motor_main();
      //printf("Count = %d\r\n", degrees_count)
    if (isFindingSpecifiedHeading) {
      max_count = DEGREES_360;
      if (abs(specifiedHeading - com->getHeading()) < HEADING_THRESHOLD) {
        max_count = DEGREES_15;
        isFindingSpecifiedHeading = false;
      }
    }
  }
}

void Motor::setDirection(bool isclock){
  isClockwise = isclock;
}

void Motor::stop(){
  isRunning = false;
}

void Motor::start(){
  isRunning = true;
  max_count = DEGREES_180;
}

void Motor::scanAtHeading(float target_degrees){
  isFindingSpecifiedHeading = true;
  specifiedHeading = target_degrees;
}
