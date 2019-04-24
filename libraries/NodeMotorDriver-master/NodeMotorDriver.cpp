/*
  NodeMotorDriver.h - Library for use with NodeMCU L293D driver board
  MIT License
*/

#include "NodeMotorDriver.h"

NodeMotorDriver::NodeMotorDriver(uint8_t pinMotorA1, uint8_t pinMotorA2, uint8_t pinMotorB1, uint8_t pinMotorB2)
{
	pinMode(pinMotorA1, OUTPUT);
	pinMode(pinMotorA2, OUTPUT);
	pinMode(pinMotorB1, OUTPUT);
	pinMode(pinMotorB2, OUTPUT);
	m_pinA1 = pinMotorA1; // speed
	m_pinA2 = pinMotorA2; // direction
	m_pinB1 = pinMotorB1; // speed
	m_pinB2 = pinMotorB2; // direction
}

void NodeMotorDriver::goForward(uint16_t pwm)
{
	analogWrite(m_pinA1, pwm);
	digitalWrite(m_pinA2, HIGH);
	analogWrite(m_pinB1, pwm);
	digitalWrite(m_pinB2, HIGH); 
}

void NodeMotorDriver::goBackward(uint16_t pwm)
{
	analogWrite(m_pinA1, pwm);
	digitalWrite(m_pinA2, LOW);
	analogWrite(m_pinB1, pwm);
	digitalWrite(m_pinB2, LOW);
}

void NodeMotorDriver::turnRight(uint16_t pwm)
{
	analogWrite(m_pinA1, pwm);
	digitalWrite(m_pinA2, HIGH);
	analogWrite(m_pinB1, pwm);
	digitalWrite(m_pinB2, LOW);
}

void NodeMotorDriver::turnLeft(uint16_t pwm)
{
	analogWrite(m_pinA1, pwm);
	digitalWrite(m_pinA2, LOW);
	analogWrite(m_pinB1, pwm);
	digitalWrite(m_pinB2, HIGH);
}

void NodeMotorDriver::stop()
{
	analogWrite(m_pinA1, 0);
	digitalWrite(m_pinA2, 0);
	analogWrite(m_pinB1, 0);
	digitalWrite(m_pinB2, 0);
}
