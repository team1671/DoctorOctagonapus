#include"Vrisk.h"

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
	//WHY BOTH???	

	LBot.ConfigEncoderCodesPerRev(kENCCOUNT);
	LTop.ConfigEncoderCodesPerRev(kENCCOUNT);
	LTop.SetVoltageRampRate(RAMP);//MAXIMUM
	LBot.SetVoltageRampRate(RAMP);//V CHANGE
									//OR I + P
	lefty.SetVoltageRampRate(RAMP);
	leftyB.SetVoltageRampRate(RAMP);
	righty.SetVoltageRampRate(RAMP);
	rightyB.SetVoltageRampRate(RAMP);
	
	LTop.EnableControl();
	LBot.EnableControl();
	rightyB.EnableControl();
	righty.EnableControl();
	leftyB.EnableControl();
	lefty.EnableControl();

	shootin = 0;
	aimin = 0;
	negate = 0;
	drive =3;
	limitedDistance = 0;
	
	setCam();
}


void DoctaEight::Autonomous(void)
{
	Kill;
	while (IsAutonomous() && IsEnabled())
	{
		UpdateCamData();
	}
}
void DoctaEight::OperatorControl(void)
{
	Kill;
	while (IsOperatorControl() && IsEnabled())
	{
		UpdateCamData();
		tardis();
	}
}
	
void DoctaEight::output (void)
{
	Kill;
	
	/*
	____.Faults(____::kCurrentFault);
	kTemperatureFault = 2, kBusVoltageFault = 4, kGateDriverFault = 8
	can use above to set fault for some things
	void CANJaguar::ConfigNeutralMode(NeutralMode) 
	fault time can also 
	*/
	
	if (IsAutonomous())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Autonomous");
	else if (IsOperatorControl())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Operator");

	if (CamData.HP == -1)
		driverOut->Printf(DriverStationLCD::kUser_Line2, 1, "no target");
	else if (CamData.LP > 4)
		driverOut->Printf(DriverStationLCD::kUser_Line2, 1, "too many targets");
	else
	{								//TRY I
		driverOut->Printf(DriverStationLCD::kUser_Line2, 1, "Number of targets: %d", CamData.numTargets);
		driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "Target selected: %d", CamData.HP);
		if (shootin)
		{
			driverOut->Printf(DriverStationLCD::kUser_Line4, 1, "Distance: %f", (double)CamData.approx);
			driverOut->PrintfLine(DriverStationLCD::kUser_Line5, "Top CANJag Temp: %f Celcius",LTop.GetTemperature()*(9/5) + 32);
			driverOut->PrintfLine(DriverStationLCD::kUser_Line6, "Bot CANJag Temp: %f Celcius",LBot.GetTemperature()*(9/5) + 32);
				
		}
		if (aimin)
			driverOut->Printf(DriverStationLCD::kUser_Line4, 1, "Double to 0: %f", CamData.centerX);
	}
	driverOut->UpdateLCD();
}

START_ROBOT_CLASS(DoctaEight);
