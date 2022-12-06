/** @file HCSRO4.cpp 
 * @brief This program contains the constructor for the ultrasonic sensor class. 
 * @author Luisa Chiu
*/

#include <Arduino.h>
#include <HCSR04.h>
#include <PrintStream.h>


/** @brief   Initializes pins.
 *  @details  Sets the Trig and Echo pins of the ultrasonic sensor.
 *  @param   p_trig Trig Pin
 *  @param p_echo Echo Pin
 */
HCSR04::HCSR04(int p_trig, int p_echo)
{
  trig_pin = p_trig;
  echo_pin = p_echo;
  pinMode(trig_pin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echo_pin, INPUT); // Sets the echoPin as an INPUT

}
// Defines variables for HC-SR04
long duration; // variable for the duration of sound wave travel
float out_dist; // variable for the distance measurement

/** @brief Constructs measure method.
 *  @details Allows for the reading of measurements obtained by the ultrasonic sensor.
*/
float HCSR04::measure()
{
  // Clears the trigPin condition
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_pin, HIGH);
  // Calculating the distance
  out_dist = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  //Serial << distance << endl;
  return out_dist/2.54; // distance in inches

}