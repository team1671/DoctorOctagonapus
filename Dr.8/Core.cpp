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
	LTop.ConfigMaxOutputVoltage(kVOLTAGE);
	LBot.ConfigMaxOutputVoltage(kVOLTAGE);
	
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
	negate = 0;
	drive =1;
	limitedDistance = 0;
	
	setCam();
}


void DoctaEight::Autonomous(void)
{
	Kill;
	while (IsAutonomous() && IsEnabled())
	{
		//say place
		UpdateCamData();
	}
}
void DoctaEight::OperatorControl(void)
{
	Kill;
	while (IsOperatorControl() && IsEnabled())
	{
		//say place
		UpdateCamData();

		tardis();//robot drive <-below
	}
}
	
void DoctaEight::output (void)
{
	Kill;
	
	if (IsAutonomous())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Autonomous");
	else if (IsOperatorControl())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Operator");
	
	if (CamData.approx != -1)
		driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "Distance: %f", (double)CamData.approx);
	else if (aimin)
		driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "Double to 0: %f", CamData.centerX);
	else
		driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "No target");
		
	
	if (CamData.HP == -1)
		driverOut->Printf(DriverStationLCD::kUser_Line2, 1, "no target");
	else if (CamData.LP > 4)
		driverOut->Printf(DriverStationLCD::kUser_Line2, 1, "too many targets");
	else
	{								//TRY I
		driverOut->Printf(DriverStationLCD::kUser_Line4, 1, "Number of targets: %d", CamData.numTargets);
		driverOut->Printf(DriverStationLCD::kUser_Line5, 1, "Target selected: %d", CamData.HP);
	}
	driverOut->UpdateLCD();
}

START_ROBOT_CLASS(DoctaEight);
