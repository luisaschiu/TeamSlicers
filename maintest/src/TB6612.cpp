#include <Arduino.h>
#include <TB6612.h>

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

void BladeMotor::fwd(int speed)
{
    digitalWrite(p_In1A, HIGH);
    digitalWrite(p_In2A, LOW);
    analogWrite(PWM_A, speed);
}

void BladeMotor::stop()
{
   digitalWrite(p_In1A, HIGH);
   digitalWrite(p_In2A, HIGH);
   analogWrite(PWM_A, 0);
}

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

void PushMotor::fwd(int speed)
{
    digitalWrite(p_In1B, LOW);
    digitalWrite(p_In2B, HIGH);
    analogWrite(PWM_B, speed);
}

void PushMotor::rev(int speed)
{
   digitalWrite(p_In1B, HIGH);
   digitalWrite(p_In2B, LOW);
   analogWrite(PWM_B, speed);
}

void PushMotor::stop()
{
   digitalWrite(p_In1B, LOW);
   digitalWrite(p_In2B, LOW);
   analogWrite(PWM_B, 0);
}

