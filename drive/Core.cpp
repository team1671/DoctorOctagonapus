#include"head.h"

DriverStationLCD* driverOut = DriverStationLCD::GetInstance();

DoctaEight::DoctaEight(void):
pilot(1),
copilot(2),
//controller(USB port)

lefty(4),
leftyB(5),
righty(2),
rightyB(3),

arm(9),
intake(8),
LTop(7),
LBot(6)
{
	Kill;
	
	lefty.ChangeControlMode(CANJaguar::kPercentVbus);
	righty.ChangeControlMode(CANJaguar::kPercentVbus);
	leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
	rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
	intake.ChangeControlMode(CANJaguar::kPercentVbus);
	arm.ChangeControlMode(CANJaguar::kPercentVbus);
	
	LTop.ChangeControlMode(CANJaguar::kSpeed);
	LBot.ChangeControlMode(CANJaguar::kSpeed);
	LBot.SetPID(KP,KI,KD);
	LTop.SetPID(KP,KI,KD);
	
	lefty.ConfigMaxOutputVoltage(kVOLTAGE);
	leftyB.ConfigMaxOutputVoltage(kVOLTAGE);
	righty.ConfigMaxOutputVoltage(kVOLTAGE);
	rightyB.ConfigMaxOutputVoltage(kVOLTAGE);
	
	LBot.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	LTop.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	LBot.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	LTop.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);

	LBot.ConfigEncoderCodesPerRev(kENCCOUNT);
	LTop.ConfigEncoderCodesPerRev(kENCCOUNT);

	lefty.SetVoltageRampRate(RAMP);
	leftyB.SetVoltageRampRate(RAMP);
	righty.SetVoltageRampRate(RAMP);
	rightyB.SetVoltageRampRate(RAMP);
	LTop.EnableControl();
	LBot.EnableControl();
//	rightyB.EnableControl();
//	righty.EnableControl();
//	leftyB.EnableControl();
//	lefty.EnableControl();

	negate = 1;
	drive =3;
}


void DoctaEight::Autonomous(void)
{
	Kill;
	while (IsAutonomous() && IsEnabled())
	{
		output();
	}
}
void DoctaEight::OperatorControl(void)
{
	Kill;
	while (IsOperatorControl() && IsEnabled())
	{
		output();
		tardis();//robot drive <-below
	}
}
	
void DoctaEight::output (void)
{
	Kill;
	
	driverOut->Clear();
	
	driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "Top CANJag Temp: %f Celcius",LTop.GetTemperature()*(9/5) + 32);
	driverOut->PrintfLine(DriverStationLCD::kUser_Line4, "Bot CANJag Temp: %f Celcius",LBot.GetTemperature()*(9/5) + 32);
	/*
	 ____.Faults(____::kCurrentFault);
	 * kTemperatureFault = 2, kBusVoltageFault = 4, kGateDriverFault = 8
	 *can use above to set fault for some things
	 * void CANJaguar::ConfigNeutralMode(NeutralMode) 
	 * fault time can also be set
	*/

	driverOut->PrintfLine(DriverStationLCD::kUser_Line5, "Top speed: %f",LTop.GetSpeed());
	driverOut->PrintfLine(DriverStationLCD::kUser_Line6, "Bot speed: %f",LBot.GetSpeed());
	
	if (IsAutonomous())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Autonomous");
	else if (IsOperatorControl())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Operator");
	driverOut->UpdateLCD();
}

START_ROBOT_CLASS(DoctaEight);
