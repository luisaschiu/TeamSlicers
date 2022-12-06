/** @file    HCSR04.h
 *  @brief   Header file for ultrasonic sensor class and measurement method.
 *  @author Luisa Chiu
 * 
 */


#ifndef HCSR04_h
#define HCSR04_h

#include <Arduino.h>

/** @brief  Class for the ultrasonic sensor. Allows for acquisition of measurement data.
 */
class HCSR04
{
  public:
  /** @brief Constructor for ultrasonic sensor
   */
  HCSR04(int trigPin, int echoPin);
  /** @brief Read data from ultrasonic sensor
   
  */
  float measure();

  private:
  // Defines variables for HC-SR04
  int trig_pin;
  int echo_pin;
  long duration; /// variable for the duration of sound wave travel
  /**
   * A variable for the distance measurement
  */
  float out_dist;

};

#endif