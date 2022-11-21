
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
  int trigPin;
  int echoPin;
  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement

};

#endif