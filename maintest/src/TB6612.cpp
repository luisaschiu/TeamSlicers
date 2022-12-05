/** @file TB6612.cpp
 *  @brief Program compiles the motor driver.
 *  @author Luisa Chiu
*/


#include <Arduino.h>
#include <TB6612.h>


/** @brief  Class for the blade motor. 
 * @details Sets the motor pins so that the correct signals can be sent from the motor driver.
 */
BladeMotor::BladeMotor(int In1pinA, int In2pinA, int PWMpinA, int STBYpin)
{
    p_In1A = In1pinA;
    p_In2A = In2pinA;
    PWM_A = PWMpinA;
    Standby = STBYpin;
  
    pinMode(p_In1A, OUTPUT);
    pinMode(p_In2A, OUTPUT);
    pinMode(PWM_A, OUTPUT);
    pinMode(Standby, OUTPUT);

    digitalWrite(Standby, HIGH);
};
/** @brief  Method for motor to spin mechanism forward. 
 * 
 */
void BladeMotor::fwd(int speed)
{
    digitalWrite(p_In1A, HIGH);
    digitalWrite(p_In2A, LOW);
    analogWrite(PWM_A, speed);
}

/** @brief  Method for motor to stop. 
 * 
 */
void BladeMotor::stop()
{
   digitalWrite(p_In1A, HIGH);
   digitalWrite(p_In2A, HIGH);
   analogWrite(PWM_A, 0);
}

/** @brief  Class for the pusher motor. 
 * @details Sets the motor pins so that the correct signals can be sent from the motor driver.
 */
PushMotor::PushMotor(int In1pinB, int In2pinB, int PWMpinB, int STBYpin)
{
    p_In1B = In1pinB;
    p_In2B = In2pinB;
    PWM_B = PWMpinB;
    Standby = STBYpin;
  
    pinMode(p_In1B, OUTPUT);
    pinMode(p_In2B, OUTPUT);
    pinMode(PWM_B, OUTPUT);
    pinMode(Standby, OUTPUT);

    digitalWrite(Standby, HIGH);
};
/** @brief  Method for motor to move the pusher forward. 
 * 
 */
void PushMotor::fwd(int speed)
{
    digitalWrite(p_In1B, LOW);
    digitalWrite(p_In2B, HIGH);
    analogWrite(PWM_B, speed);
}
/** @brief  Method for motor to move the pusher backwards. 
 * 
 */
void PushMotor::rev(int speed)
{
   digitalWrite(p_In1B, HIGH);
   digitalWrite(p_In2B, LOW);
   analogWrite(PWM_B, speed);
}
/** @brief  Method for motor to stop. 
 * 
 */
void PushMotor::stop()
{
   digitalWrite(p_In1B, LOW);
   digitalWrite(p_In2B, LOW);
   analogWrite(PWM_B, 0);
}

