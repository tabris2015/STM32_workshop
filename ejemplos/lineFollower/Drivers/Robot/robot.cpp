#include "robot.h"

Robot::Robot(RobotPins * pins): _pins(pins)
{
    _port = pins->port;
}
void Robot::drive(int16_t speed)
{
	if(speed > 0)
	{
		leftForward((uint16_t)abs((int16_t)speed));
		rightForward((uint16_t)abs((int16_t)speed));
	}
	else
	{
		leftBackward((uint16_t)abs((int16_t)speed));
		rightBackward((uint16_t)abs((int16_t)speed));	
	}
}

void Robot::drive(int16_t l_speed, int16_t r_speed)
{
	if(l_speed > 0)
	{
		leftForward((uint16_t)abs((int16_t)l_speed));
	}
	else
	{
		leftBackward((uint16_t)abs((int16_t)l_speed));	
	}

	if(r_speed > 0)
	{
		rightForward((uint16_t)abs((int16_t)r_speed));
	}
	else
	{
		rightBackward((uint16_t)abs((int16_t)r_speed));	
	}	
}

void Robot::drive(float l_speedf, float r_speedf)
{
	uint32_t l_speed = (uint32_t)(CONV_RATE * l_speedf);
	uint32_t r_speed = (uint32_t)(CONV_RATE * r_speedf);
	
	if(l_speed > 0)
	{
		leftForward((uint16_t)abs((int16_t)l_speed));
	}
	else
	{
		leftBackward((uint16_t)abs((int16_t)l_speed));	
	}

	if(r_speed > 0)
	{
		rightForward((uint16_t)abs((int16_t)r_speed));
	}
	else
	{
		rightBackward((uint16_t)abs((int16_t)r_speed));	
	}	
}


void Robot::differentialDrive(float linear, float angular)
{
	const float factor = 0.7;

    float v_right = (1 - factor) * linear - factor * angular;
    float v_left = (1 - factor) * linear + factor * angular;
    drive(v_left, v_right);

}

void Robot::pivot(int16_t speed)
{
	if(speed > 0)
	{
		leftBackward((uint16_t)abs((int16_t)speed));
		rightForward((uint16_t)abs((int16_t)speed));
	}
	else
	{
		leftForward((uint16_t)abs((int16_t)speed));
		rightBackward((uint16_t)abs((int16_t)speed));	
	}	
}
void Robot::stop()
{
	PWM_A = 0;
    PWM_B = 0;
}
void Robot::leftForward(uint16_t speed)
{
  
    _port->BSRR = _pins->ma_s1;
    _port->BSRR = (uint32_t)(_pins->ma_s2) << 16;
    PWM_A = speed;
}
void Robot::leftBackward(uint16_t speed)
{
	_port->BSRR = _pins->ma_s2;
    _port->BSRR = (uint32_t)(_pins->ma_s1) << 16;
    PWM_A = speed;
}
void Robot::rightForward(uint16_t speed)
{
	_port->BSRR = _pins->mb_s1;
    _port->BSRR = (uint32_t)(_pins->mb_s2) << 16;
    PWM_B = speed;
}
void Robot::rightBackward(uint16_t speed)
{
	_port->BSRR = _pins->mb_s2;
    _port->BSRR = (uint32_t)(_pins->mb_s1) << 16;
    PWM_B = speed;
}

float Robot::_map(float x, float inMin, float inMax, float outMin, float outMax)
{
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
