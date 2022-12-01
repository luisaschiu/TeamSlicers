#include <Arduino.h>
#include <PrintStream.h>
#include <TB6612.h>
#include <HCSR04.h>
#include <taskshare.h>
#include <shares.h>
#include <task_webserver.h>

#define AIN1 25 // Blue Wire
#define BIN1 26 // Green Wire
#define AIN2 33 // Black Wire
#define BIN2 27 // Orange Wire
#define PWMA 32
#define PWMB 14
#define STBY 4
#define CH_A 0 // Yellow Wire ENCODER 
#define CH_B 15 // Blue Wire ENCODER 
#define BLADELIMIT_PIN 34
#define HOMEPUSHLIMIT_PIN 39
#define ENDPUSHLIMIT_PIN 36
#define trigPin 17
#define echoPin 16

Share<float> distance ("HCSR04 Output");
Share<bool> start_flag ("Start Button"); //used to be ready_flag
Share<bool> push_flag ("Motor Communication Bool");
Share<bool> user_cut_flag ("User Input/Start Cutting");
Share<bool> home_pusher_flag ("Home Pusher Bool");
Share<bool> home_blade_flag ("Home Blade Bool");
Share<bool> done_flag ("Slicing Finished Bool");
Share<int> thickness_option ("Option for Different Thicknesses");

PushMotor motor1 = PushMotor(BIN1, BIN2, PWMB, STBY);

void task_mot1(void* param)
{
    int state = 0;
    int counter = 0;
    for(;;)
    {
        Serial << analogRead(HOMEPUSHLIMIT_PIN) << endl;
        counter ++;
        if (state == 0)
        {
            Serial << "state 0" << endl;
            motor1.fwd(255);
            if (analogRead(HOMEPUSHLIMIT_PIN) == 0)
            {
                Serial << "hi" << endl;
                state = 1;
            }
        }
        else if (state == 1)
        {
            Serial << "state 1" << endl;
            motor1.rev(255);
            if (analogRead(HOMEPUSHLIMIT_PIN) > 0)
            {
                state = 0;
            }
        }

    vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 100 ms
    }
}

void task_motor1(void* param)
{
    int state = 0;
    int counter = 0;
    for(;;)
    {
//        Serial << "still looping" << endl;
        if (state == 0)
        {
            motor1.stop();
            Serial << "State 0" << endl;
            if ((home_pusher_flag.get() == true) and (analogRead(HOMEPUSHLIMIT_PIN) > 0))
            {
                state = 1;
                
            }
            else if ((home_pusher_flag.get() == true) and (analogRead(HOMEPUSHLIMIT_PIN) == 0))
            {
                state = 2;
            }

        }
        else if (state == 1)
        {
            Serial << "State 1" << endl;
            motor1.rev(255);
            if (analogRead(HOMEPUSHLIMIT_PIN) == 0)
            {
                state = 2;
            }
        }
        else if (state == 2)
        {
            Serial << "State 2" << endl;
            motor1.stop();
            if (start_flag.get() == true);
            {
                state = 3;
            }
        }
        else if (state == 3)
        {
            Serial << "State 3" << endl;
            motor1.fwd(255);
        //    Serial << distance.get() << endl;
            if (distance.get() <= 2.00)  // UPDATE VALUE TO PUSH OBJECT TO BLADE
            {
                state = 4;
            }
        }
        else if (state == 4)
        {
            Serial << "State 4" << endl;
            motor1.stop();
            if ((user_cut_flag.get() == true) and (thickness_option.get() == 1))
            {
                state = 5;
            }
            else if ((user_cut_flag.get() == true) and (thickness_option.get() == 2))
            {
                state = 6;
            }
            else if ((user_cut_flag.get() == true) and (thickness_option.get() == 3))
            {
               
                state = 7;
            }
        }
        else if (state == 5)
        {
            Serial << "State 5" << endl;
            push_flag.put(true);
            motor1.fwd(255);
            counter ++;
            if (counter == 13) // counter = 13, loaded: 0.25 in travel
            {  
                push_flag.put(false);
                state = 8;
            }
            else if (analogRead(ENDPUSHLIMIT_PIN) == 0)
            {
//                done_flag.put(true);
                state = 0;
            }
        }
        else if (state == 6)
        {
            Serial << "State 6" << endl;
            push_flag.put(true);
            motor1.fwd(255);
            counter ++;
            if (counter == 26) // counter = 26, loaded: 0.5 in travel
            {  
                push_flag.put(false);
                state = 8;
            } 
            else if (analogRead(ENDPUSHLIMIT_PIN) == 0)
            {
//                done_flag.put(true);
                state = 0;
            }
        }
        else if (state == 7)
        {
            Serial << "State 7" << endl;
            push_flag.put(true);
            motor1.fwd(255);
            counter ++;
            if (counter == 52) // counter = 52, loaded: 1 in travel
            {  
                push_flag.put(false);
                state = 8;
            } 
            else if (analogRead(ENDPUSHLIMIT_PIN) == 0)
            {
//                done_flag.put(true);
                state = 0;
            }
        }
        else if (state == 8)
        {
            Serial << "State 8" << endl;
            motor1.stop();
            if ((thickness_option.get() == 1) and (push_flag.get() == true))
            {
                counter = 0;
                state = 5;
            }
            else if ((thickness_option.get() == 2) and (push_flag.get() == true))
            {
                counter = 0;
                state = 6;
            }
            else if ((thickness_option.get() == 3) and (push_flag.get() == true))
            {
                counter = 0;
                state = 7;
            }
        }
     vTaskDelay(100/portTICK_PERIOD_MS); //Delay for 100 ms
    }
}

BladeMotor motor2 = BladeMotor(AIN1, AIN2, PWMA, STBY);

void task_motor2(void* param)
{
//    pinMode(BLADELIMIT_PIN, INPUT);
  //  int test_home = 0;
    //int test_push = 0;
    int state = 0;
    int counter = 0;
    //int click = 0;
    for(;;)
    {   
//        Serial << analogRead(BLADELIMIT_PIN) << endl;
        if (state == 0)
        {
            Serial << "state 0" << endl;
     //       test_home ++;
    //        if (test_home >= 1)
            if (home_blade_flag.get() == true)
            {
                state = 1;
            }
        }
        else if (state == 1)
        { 
            Serial << "state 1" << endl;
            motor2.fwd(255);
//            test_home = 0;
            if (analogRead(BLADELIMIT_PIN) == 0)
            {
                push_flag.put(true);
                state = 2;
   //             test_push = 0;
            }
        }
        else if (state == 2)
        {
            Serial << "state 2" << endl;
            motor2.stop();
   //         test_push ++; // time it takes to push forward object
  //          if (test_push >= 2000)
            if (push_flag.get() == false)
            {
   //             counter = 0;
                state = 3;
            }

        }
        else if (state == 3)
        {
            Serial << "state 3" << endl;
            motor2.fwd(255);
            counter ++;
            if (counter >= 300)
            {    
 //               test_push = 0;
                state = 1;
                counter = 0;
            }
            
        }
        
// ALTERNATE METHOD: Deadman Switch
  /*    //  Serial << analogRead(BLADELIMIT_PIN) << endl;
        if (state == 0)
        {
            Serial << "state 0" << endl;
            if (start_flag.get() == true)
                {
                    state = 1;
                }
            else if (home_blade_flag.get() == true)
                {
                    state = 2;
                }
        }
        else if (state == 1)
        {
//            Serial << "state 1" << endl;
            motor2.stop();
//            home_blade_flag.put(false);
//                test_flag = false;
//            if (done_flag.get() == true)
//            { 
//               state = 0;
//            }
//            else if (push_flag.get() == false)
            Serial << "State 1"<< endl;
            counter++;
            if (analogRead(BLADELIMIT_PIN) == 0)
            {
                click = 1;
            }
            if (analogRead(BLADELIMIT_PIN) > 0)
            {
                click = 3;
            }
            else if (click == 1 and counter >= 25)
            {
                counter = 0;
                state = 2;
            }
        }
        else if (state == 2)
        {
        Serial << "state 2" << endl;
            motor2.fwd(255);
            counter++;
            if (analogRead(BLADELIMIT_PIN) == 0)
            {
//                push_flag.put(true);
                click = 2;
            }
            if (analogRead(BLADELIMIT_PIN) > 0)
            {
                click = 3;
            }
            else if (click ==2 and counter >= 25) 
            {
                state =1;
                counter =0;
            }
        }
        */

    }
    vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 20 ms
    }


HCSR04 ultrasonic = HCSR04(trigPin, echoPin);
void task_ultrasonic(void* param)
{
    int state = 0;
    float measurement;
    for(;;)
    {
        if (state == 0)
        {
            if (start_flag.get() == true)
                {
                    state = 1;
                }
        }
        else if (state == 1)
        {
            measurement = ultrasonic.measure();
            distance.put(measurement);
        //    Serial << measurement << endl;
            vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 20 ms
            if (measurement <= 0.00)
                state = 0;
        }

    }
}

void setup() 
{

  // Begin Serial port
  Serial.begin(115200);
  setup_wifi (); // starts wifi
  // Create task objects and run tasks
    xTaskCreate (task_motor1, "PushMotor", 3000, NULL, 1, NULL);
  //  xTaskCreate (task_mot1, "PushMotor", 3000, NULL, 1, NULL);
//  xTaskCreate (task_motor2, "BladeMotor", 3000, NULL, 2, NULL);
    xTaskCreate (task_webserver, "Web Server", 8192, NULL, 3, NULL);
//  xTaskCreate (task_ultrasonic, "Ultrasonic", 5000, NULL, 4, NULL);
}

void loop() 
{
    // Delay for a whole minute, which is an eternity to a microcontroller
    vTaskDelay (60000);
}
