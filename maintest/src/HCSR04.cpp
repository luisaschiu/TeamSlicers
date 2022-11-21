#include <Arduino.h>
#include <HCSR04.h>
#include <PrintStream.h>

HCSR04::HCSR04(int p_trig, int p_echo)
{
  trigPin = p_trig;
  echoPin = p_echo;
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(trigPin, INPUT); // Sets the echoPin as an INPUT

}
// Defines variables for HC-SR04
long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement


float HCSR04::measure()
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2*100; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial << distance << endl;
  return distance; // distance in mm

}