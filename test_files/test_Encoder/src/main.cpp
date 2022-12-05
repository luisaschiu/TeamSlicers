#include <Arduino.h>
#include <ESP32Encoder.h> 
 
#define CH_A 0 // Yellow Wire ENCODER 
#define CH_B 12 // Blue Wire ENCODER 
 
ESP32Encoder encoder;
 
void setup () { 
  encoder.attachHalfQuad ( CH_A, CH_B );
  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.setCount ( 0 ); 
  Serial.begin ( 115200 );
  for (;;)
  {
    long newPosition = encoder.getCount() / 2;
    Serial.println(newPosition);
    vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 20 ms
  }
}
 
void loop () 
{    
    // Delay for a whole minute, which is an eternity to a microcontroller
    vTaskDelay (60000);
} 