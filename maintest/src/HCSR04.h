
#ifndef HCSR04_h
#define HCSR04_h

#include <Arduino.h>

class HCSR04
{
  public:
  //Constructor for ultrasonic sensor
  HCSR04(int trigPin, int echoPin);
  float measure();

  private:
  // Defines variables for HC-SR04
  int trig_pin;
  int echo_pin;
  long duration; // variable for the duration of sound wave travel
  float out_dist; // variable for the distance measurement

};

#endif