#include "Lift.h"

Lift::Lift(void)
{
	m_Lifter = NULL;
};

Lift::Lift(int canport)
{
	m_Lifter = new CANJaguar(canport);
	m_Lifter->ChangeControlMode(CANJaguar::kPercentVbus);
};

void Lift::Control(Joystick *stick)
{
	m_Lifter->Set(-stick->GetY()); 	//forward INTAKE
};									//backward OUTTAKE 
