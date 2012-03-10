#ifndef LIFT_H
#define LIFT_H

#include"WPILib.h"

class Lift
{
private:
	CANJaguar *m_Lifter;
	
public:	
	Lift(void);
	Lift(int canport);
	void Control(Joystick *stick);
	void AutonomousLift(bool firstExecution);
	
};

#endif
