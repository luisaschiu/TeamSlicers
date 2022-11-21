#include <Arduino.h>
#include <PrintStream.h>
#define LIMIT_SWITCH_PIN 34

void setup() {
  Serial.begin(115200);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
}
 
void loop() {
//  Serial.println(analogRead(LIMIT_SWITCH_PIN));
  Serial.println(analogRead(LIMIT_SWITCH_PIN));
  if (analogRead(LIMIT_SWITCH_PIN) > 0)
  {
    Serial.println("Activated!");
  }
  else
  {
    Serial.println("Not activated.");
  }
  delay(100);
}