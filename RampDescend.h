<<<<<<< HEAD
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
	void AutonomousArm(float rightArm);
};

#endif
=======
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
	void AutonomousArm(float rightArm);
};

#endif
>>>>>>> b575175822067cc77622453ead236f74494a7ac7
