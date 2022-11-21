#include <Arduino.h>
#include <PrintStream.h>
#include <TB6612.h>
//#include <HCSR04.h>

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

PushMotor motor1 = PushMotor(BIN1, BIN2, PWMB, STBY);

void task_motor1(void* param)
{
    int state = 1;
    int counter = 0;
    //Serial << "Going to state 0";
    for(;;)
    {
        if (state == 1)
        {
            motor1.rev(255);
            if (analogRead(HOMELIMIT_PIN) > 0)
            {
                state = 2;
            }
        }
        else if (state == 2)
        {
            motor1.stop();
            state = 5;
        }
        else if (state == 5)
        {
            motor1.fwd(255);
            counter ++;
            if (counter == 13) // counter = 13, loaded: 0.25 in travel
            {  
                state = 8;
            } 
        }
        else if (state == 6)
        {
            motor1.fwd(255);
            counter ++;
            if (counter == 26) // counter = 26, loaded: 0.5 in travel
            {  
                state = 8;
            } 
        }
        else if (state == 7)
        {
            motor1.fwd(255);
            counter ++;
            if (counter == 52) // counter = 52, loaded: 1 in travel
            {  
                state = 8;
            } 
        }
        else if (state == 8)
        {
            motor1.stop();
            state = 8;
        }
        else if (state == 9)
        {
        }
     vTaskDelay(100/portTICK_PERIOD_MS); //Delay for 100 ms
    }
}

BladeMotor motor2 = BladeMotor(AIN1, AIN2, PWMA, STBY);

void task_motor2(void* param)
{
    //int state = 0;
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
  //xTaskCreate (task_encoder, "Encoder", 5000, NULL, 1, NULL);
  xTaskCreate (task_motor1, "PushMotor", 3000, NULL, 2, NULL);
  //xTaskCreate (task_motor2, "BladeMotor", 3000, NULL, 1, NULL);
}

void loop() 
{
    // Delay for a whole minute, which is an eternity to a microcontroller
    vTaskDelay (60000);
}
