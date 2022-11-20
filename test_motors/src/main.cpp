#include <Arduino.h>
#include <PrintStream.h>
#include <TB6612.h>
#include <ESP32Encoder.h>

#define AIN1 25
#define BIN1 26
#define AIN2 33
#define BIN2 27
#define PWMA 32
#define PWMB 14
#define STBY 4
#define CH_A 0 // Yellow Wire ENCODER 
#define CH_B 15 // Blue Wire ENCODER 
 
ESP32Encoder encoder;
 
void task_encoder(void* param)
{ 
    encoder.attachHalfQuad ( CH_A, CH_B );
    ESP32Encoder::useInternalWeakPullResistors=UP;
    encoder.setCount (0); 
    for (;;)
    {
        long newPosition = encoder.getCount() / 2;
        Serial.println(newPosition);
        vTaskDelay(80/portTICK_PERIOD_MS); //Delay for 50 ms
    }
}

PushMotor motor1 = PushMotor(BIN1, BIN2, PWMB, STBY);

void task_motor1(void* param)
{
    int state = 0;
    Serial << "Going to state 0";
    for(;;)
    {
        if (state == 0)
        {
            Serial << "In state 0";
            motor1.fwd(255);
            if ((encoder.getCount()/2) >= 4097)
            {  
                Serial << "Going to state 1";
                state = 1;
            } 
        }
        else if (state == 1)
        {
            motor1.stop();
            Serial << "MOT STOP";
        }
        vTaskDelay(1000/portTICK_PERIOD_MS); //Delay for 10 sec
    }
}

BladeMotor motor2 = BladeMotor(AIN1, AIN2, PWMA, STBY);

void task_motor2(void* param)
{
    int state = 0;
    for(;;)
    {
        motor2.fwd(255);
        vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 20 ms
    }
}

void setup() 
{
  // Begin Serial port
  Serial.begin(115200);
  // Create task objects and run tasks
  xTaskCreate (task_encoder, "Encoder", 5000, NULL, 1, NULL);
  xTaskCreate (task_motor1, "PushMotor", 3000, NULL, 2, NULL);
}

void loop() 
{
    // Delay for a whole minute, which is an eternity to a microcontroller
    vTaskDelay (60000);
}
