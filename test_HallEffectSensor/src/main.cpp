#include <Arduino.h>
#include <PrintStream.h>
#define HallEff_Sensor 34

void setup() 
{
  Serial.begin(115200);
  int state = 0;
  pinMode(HallEff_Sensor, INPUT_PULLUP);
  for (;;)
  {
  state = digitalRead(HallEff_Sensor);
  if (state == LOW)
  {
    Serial << "LOW" << endl;
  }
  else
  {
    Serial << "HIGH" << endl;;
  }
  vTaskDelay(50/portTICK_PERIOD_MS); //Delay for 20 ms
  }
}


void loop() 
{
  // Delay for a whole minute, which is an eternity to a microcontroller
  vTaskDelay (60000);
}