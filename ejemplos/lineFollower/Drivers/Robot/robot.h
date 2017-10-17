#ifndef ROBOT_H
#define ROBOT_H
#include <stdint.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"

#define PWM_TIM TIM4
#define PWM_A (PWM_TIM->CCR4)
#define PWM_B (PWM_TIM->CCR3)
#define MAX_PWM 2000
#define MAX_F_PWM 100.0
#define CONV_RATE (MAX_PWM / MAX_F_PWM)

typedef struct RobotPins_t
{
    GPIO_TypeDef * port;
    uint16_t ma_s1;
    uint16_t ma_s2;
    uint16_t mb_s1;
    uint16_t mb_s2;
}RobotPins;

class Robot
{
public:
	/* Constructor takes the pins of the driver */
	Robot(RobotPins * pins);			

	/* Drive both motors with given speed, max speed is 255 (resolution of PWM on arduino UNO, NANO, PRO MINI, etc) */
	void drive(int16_t speed);  
	/* Drive the motors with different speeds */
    void drive(int16_t l_speed, int16_t r_speed);
    /* the same with a float value between 0 and 100.0 */
    void drive(float l_speedf, float r_speedf);
    /* Drive motors in opposite directios */
    void pivot(int16_t speed);
    /* Stop both motors*/
    void stop();
    /* Drive both motors following a differential drive model
    	this need to be refined with the model of the robot */
    void differentialDrive(float linear, float angular);
    
  private:
  	/* Helper functions for driving individual motors*/
  	/* Drive left motor forward */
    void leftForward(uint16_t speed);
    /* Drive left motor backwards */
    void leftBackward(uint16_t speed);
    /* Drive right motor forward */
    void rightForward(uint16_t speed);
    /* Drive right motor backwards */
    void rightBackward(uint16_t speed);
    float _map(float x, float inMin, float inMax, float outMin, float outMax);
    //pin struct
    RobotPins * _pins;
    GPIO_TypeDef * _port;
};
#endif //ROBOT_H
