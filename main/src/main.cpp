/** @file main.cpp
 *  @brief Main file for the automatic cutting board. 
 *  @author Luisa Chiu
 *  @author Kevin Salceda
 *  @author Caleb Kephart 
 * 
*/

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
#define trigPin 17 //white wire 
#define echoPin 16 // Green wire 

Share<float> distance ("HCSR04 Output");
Share<bool> start_flag ("Start Button");
Share<bool> push_flag ("Motor Communication Bool");
Share<bool> user_cut_flag ("User Input/Start Cutting");
Share<bool> home_pusher_flag ("Home Pusher Bool");
Share<bool> home_blade_flag ("Home Blade Bool");
Share<bool> done_flag ("Slicing Finished Bool");
Share<int> thickness_option ("Option for Different Thicknesses");


/** @brief initializes the push motor class.
 * 
*/
PushMotor motor1 = PushMotor(BIN1, BIN2, PWMB, STBY);

/** @brief Task for the pusher motor.
 *  @details Handles all actions for the pusher motor. Reads shares changed by the user interface webserver. 
 *           Based on User interface, the motor is pushing for a certain period of time, then stops. It then waits for the blade 
 *           task to finish the cut, then moves. This repeats until the pusher hits the limit switch at the end of its travel.
 * 
*/
void task_motor1(void* param)
{
    int state = 0;
    int counter = 0;
    for(;;)
    {
        if (state == 0)
        {
            motor1.stop();
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
            motor1.rev(255);
            if (analogRead(HOMEPUSHLIMIT_PIN) == 0)
            {
                state = 2;
            }
        }
        else if (state == 2)
        {
            Serial << " 2" << endl;
        
            motor1.stop();
            if (start_flag.get() == true);
            {
                state = 3;
            }
        }
        else if (state == 3)
        {
           Serial << " 3" << endl;
            motor1.fwd(255);
            if (distance.get() <= 1)  
            {
                state = 4;
            }
        }
        else if (state == 4)
        {
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
           
            push_flag.put(true);
            motor1.fwd(255);
            counter ++;
            if (counter == 28) 
            {  
                push_flag.put(false);
                state = 8;
            }
            else if (analogRead(ENDPUSHLIMIT_PIN) == 0)
            {
                
            thickness_option.put(0);
            home_blade_flag.put(false);
            home_pusher_flag.put(false);
            user_cut_flag.put(false);
                state = 0;
            }
        }
        else if (state == 6)
        {
            push_flag.put(true);
            motor1.fwd(255);
            counter ++;
            if (counter == 52) 
            {  
                push_flag.put(false);
                state = 8;
            } 
            else if (analogRead(ENDPUSHLIMIT_PIN) == 0)
            {
            thickness_option.put(0);
            home_blade_flag.put(false);
            home_pusher_flag.put(false);
            user_cut_flag.put(false);
                state = 0;
            }
        }
        else if (state == 7)
        {
            push_flag.put(true);
            motor1.fwd(255);
            counter ++;
            if (counter == 104) 
            {  
                push_flag.put(false);
                state = 8;
            } 
            else if (analogRead(ENDPUSHLIMIT_PIN) == 0)
            {

            thickness_option.put(0);
            home_blade_flag.put(false);
            home_pusher_flag.put(false);
            user_cut_flag.put(false);
                state = 0;
            }
        }
        else if (state == 8)
        {
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
     vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 100 ms
    }
}

/** @brief Initializes the blademotor task.
 * 
*/
BladeMotor motor2 = BladeMotor(AIN1, AIN2, PWMA, STBY);
/** @brief Task for the blade motor.
 *  @details Handles all actions for the blade motor. Reads shares changed by the user interface webserver. 
 *           The motor is capable of homing itself when asked to by the user. It then waits for the pusher 
 *           task to finish its motion, then moves and slices the produce. This repeats until the pusher hits the limit switch at the end of its travel.
 * 
*/
void task_motor2(void* param)
{

    int state = 0;
    int counter = 0;
    
    for(;;)
    {   

        if (state == 0)
        {
     
            if (home_blade_flag.get() == true)
            {
                state = 1;
            }
        }
        else if (state == 1)
        { 
           Serial << "state 1" << endl;
            motor2.fwd(255);

            if (analogRead(BLADELIMIT_PIN) == 0)
            {
                push_flag.put(true);
                state = 2;

            }
        }
        else if (state == 2)
        {
          Serial << "state 2" << endl;
            motor2.stop();

            if (push_flag.get() == false)
            {

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
 
                state = 1;
                counter = 0;
            }
            
        }
        


    }
    vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 20 ms
    }

/** @brief Initializes class for ultrasonic sensor
 * 
*/
HCSR04 ultrasonic = HCSR04(trigPin, echoPin);
/** @brief Task for ultrasonic sensor
 *  @details Ultrasonic measures the distance between the blade and the start of the object to be sliced. 
 *          Pusher brings up the produce until it trips the ultrasonic sensor.
 * 
*/

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
            measurement = ultrasonic.measure()-1.76;
            distance.put(measurement);
       
            if (measurement <= 0.00)
                {
                    start_flag.put(false);
                    state = 0;
                }
        }
    vTaskDelay(20/portTICK_PERIOD_MS); //Delay for 20 ms
    }
}

void setup() 
{

  // Begin Serial port
  Serial.begin(115200);
  setup_wifi (); // starts wifi
  // Create task objects and run tasks
    xTaskCreate (task_motor1, "PushMotor", 3000, NULL, 1, NULL);
    xTaskCreate (task_motor2, "BladeMotor", 3000, NULL, 2, NULL);
   xTaskCreate (task_webserver, "Web Server", 8192, NULL, 3, NULL);
   xTaskCreate (task_ultrasonic, "Ultrasonic", 5000, NULL, 4, NULL);
}

void loop() 
{
    // Delay for a whole minute, which is an eternity to a microcontroller
    vTaskDelay (60000);
}
