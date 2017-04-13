#include "Ultrasonic_interrupt.h"

Ultrasonic::Ultrasonic(DigitalOut * triggerIn, InterruptIn * echoIn, Radar * radarIn, Motor *motorIn){
  radar = radarIn;
  trigger = triggerIn;
  echo = echoIn;
  motorP = motorIn;
  timer.start();
  interruptStart();
}

void Ultrasonic::timerStart() {
    if(timer.read()>600) {
        timer.reset ();
    }
    start = timer.read_us ();
}

void Ultrasonic::interruptStart(void) {
      timeout.detach();
      *trigger=1;
      wait_us(10);
      done = 0;
      echo->rise(this,&Ultrasonic::timerStart);
      echo->fall(this,&Ultrasonic::updateDistance);
      echo->enable_irq ();
      timeout.attach(this,&Ultrasonic::interruptStart,1);
      *trigger=0;
}

void Ultrasonic::updateDistance() {
    end = timer.read_us ();
    done = 1;
    distance = (end - start)/6;
    timeout.detach();
    timeout.attach(this,&Ultrasonic::interruptStart, 0.01);
}

string Ultrasonic::intToString ( int number ) {
  char result[16]; // string which will contain the number
  sprintf ( result, "%d", number ); // %d makes the result be a decimal integer
  return result;
}

void Ultrasonic::callback(){
  tempDistance=done;
  done = 0;

  distance  = distance / 15;

  if(distance > 64){
    //abort
    return;
  }


  if(tempDistance) {



    int degree = motorP->get_degrees();
    bool x_right = (degree > 90);
    degree = abs(degree - 90);
    float third_degree = 90 - degree; //degree for third corner of triangle and the opposite one

    float hypotenuse_2 = distance * (cos (degree*PI/180.0));

    float x = hypotenuse_2 * (cos (third_degree*PI/180.0));
    float y = hypotenuse_2 * (sin (third_degree*PI/180.0));

    //abs of dist -64
    y = abs(y - 64);

    if(x_right)
      x = 64 + x;
    else
      x = 64 - x;

    string name = "pin_" + intToString(x) + "_" + intToString(y);
    // int y = 64;
    // printf("distance %d %s (%d,%d)\r\n", distance, name, x,y);
    radar->addRadarPin(RadarPin(name, x, y));
  }

}
