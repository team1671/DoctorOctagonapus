#include "RampDescend.h"

RampDescend::RampDescend(void)
{
	m_canRampDescend = NULL;
};

RampDescend::RampDescend(int _nRampDescend_port)
{
	m_canRampDescend = new CANJaguar(_nRampDescend_port);
	m_canRampDescend->ChangeControlMode(CANJaguar::kPercentVbus);
};

void RampDescend::Control(Joystick *stick)
{
		if (stick->GetRawButton(7))//LT
		{
			m_canRampDescend -> Set(-0.55);
		}
		else if (stick->GetRawButton(8))//RT
		{
			m_canRampDescend -> Set(0.5);
		}
		else 
		{
			m_canRampDescend -> Set(0.0);
		}
}

void RampDescend::AutonomousArm(float rightArm)//int objective
{
//	if (objective == -1)
//		m_canRampDescend -> Set(0.55);
//	else if (objective == 1)
//		m_canRampDescend -> Set(-0.55);
//	else
//		m_canRampDescend -> Set(0);
	m_canRampDescend -> Set(rightArm);
}
