/** @file TB6612.h
 *  @brief Constructor file for motor driver
 *  @author Luisa Chiu
*/

#ifndef TB6612_h
#define TB6612_h

#include <Arduino.h>

class BladeMotor
{
    public:
    //Constructor for motor
    BladeMotor(int In1pin, int In2pin, int PWMpin, int STBYpin);
	void fwd(int speed);
    void stop();
    
    private:
    //variables for the 2 inputs, PWM input, Offset value, and the Standby pin
	int p_In1A, p_In2A, PWM_A, Standby;

};

class PushMotor
{
    public:
    //Constructor for motor
    PushMotor(int In1pin, int In2pin, int PWMpin, int STBYpin);
	void fwd(int speed);
    void rev(int speed);
    void stop();
    
    private:
    //variables for the 2 inputs, PWM input, Offset value, and the Standby pin
	int p_In1B, p_In2B, PWM_B, Standby;

    int index = 0;

};

#endif