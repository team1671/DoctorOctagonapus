#ifndef RAMP_DESCEND_H_
#define RAMP_DESCEND_H_

#include "WPILib.h"

class RampDescend
{
private:
	CANJaguar	*m_canRampDescend;	
	
public:
	RampDescend(void);
	RampDescend(int _nRampDescend_port);
	void Control(Joystick *stick);
	void AutonomousArm(int objective);
};

#endif
