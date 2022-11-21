#include <Arduino.h>
#include <PrintStream.h>
#include <TB6612.h>

#define AIN1 25 //
#define BIN1 26 // Green Wire
#define AIN2 33 //
#define BIN2 27 // Orange Wire
#define PWMA 32
#define PWMB 14
#define STBY 4
#define CH_A 0 // Yellow Wire ENCODER 
#define CH_B 15 // Blue Wire ENCODER 
#define HOMELIMIT_PIN 34
 
BladeMotor motor2 = BladeMotor(AIN1, AIN2, PWMA, STBY);

void task_motor2(void* param)
{
    for(;;)
    {
        motor2.fwd(255);
        Serial << "MOTOR ON";
        vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 20 ms
    }
}

void setup() 
{
  // Begin Serial port
  Serial.begin(115200);
  // Create task objects and run tasks
  xTaskCreate (task_motor2, "BladeMotor", 3000, NULL, 1, NULL);
}

void loop() 
{
    // Delay for a whole minute, which is an eternity to a microcontroller
    vTaskDelay (60000);
}
