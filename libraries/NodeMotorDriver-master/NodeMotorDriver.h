/*
  NodeMotorDriver.h - Library for use with NodeMCU L293D driver board
  MIT License
*/

// ensure this library description is only included once
#ifndef NodeMotorDriver_h
#define NodeMotorDriver_h

#include <Arduino.h>

class NodeMotorDriver
{
	public:
		NodeMotorDriver(uint8_t pinMotorA1, uint8_t pinMotorA2, uint8_t pinMotorB1, uint8_t pinMotorB2);
		void goForward(uint16_t pwm);
		void goBackward(uint16_t pwm);
		void turnRight(uint16_t pwm);
		void turnLeft(uint16_t pwm);
		void stop();	

	private:
		uint8_t m_pinA1;
		uint8_t m_pinA2;
		uint8_t m_pinB1;
		uint8_t m_pinB2;
};

#endif
