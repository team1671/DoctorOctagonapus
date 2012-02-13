#include"Doc8.h"

static AxisCamera &eyeOfSauron = AxisCamera::GetInstance("10.16.71.11");

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
	shootin = 0;
	negate = 0;
	drive =1;
	
	lefty.ChangeControlMode(CANJaguar::kPercentVbus);
	righty.ChangeControlMode(CANJaguar::kPercentVbus);
	leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
	rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
	intake.ChangeControlMode(CANJaguar::kPercentVbus);
	arm.ChangeControlMode(CANJaguar::kPercentVbus);
	
	LTop.ChangeControlMode(CANJaguar::kSpeed);
	LBot.ChangeControlMode(CANJaguar::kSpeed);
	LBot.ConfigEncoderCodesPerRev(ENCCOUNT);
	LTop.ConfigEncoderCodesPerRev(ENCCOUNT);
//		LTop.SetVoltageRampRate(RAMP);
//		LBot.SetVoltageRampRate(RAMP);
//VAGETA!	
	
	LBot.SetPID(KP,KI,KD);
	LTop.SetPID(KP,KI,KD);
	
	
	//VAGETA THERE IS NO WAY THAT CAN BE RIGHT
//		lefty.SetVoltageRampRate(RAMP);
//		leftyB.SetVoltageRampRate(RAMP);
//		righty.SetVoltageRampRate(RAMP);
//		rightyB.SetVoltageRampRate(RAMP);
//		LTop.SetVoltageRampRate(RAMP);
//		LBot.SetVoltageRampRate(RAMP);

	//eyeOfSauron.WriteBrightness(30);
	//eyeOfSauron.WriteWhiteBalance(AxisCamera::kWhiteBalance_Hold);
	//eyeOfSauron.WriteResolution(AxisCamera::kResolution_640x360);
	//eyeOfSauron.WriteColorLevel(100);
	
	//eyeOfSauron.WriteCompression(30);
	//eyeOfSauron.WriteMaxFPS(5);
	//lower easier on CRIO and harder on cam
	
	
	limitedDistance = 0;
}


void DoctaEight::Autonomous(void)
{
	Kill;
	while (IsAutonomous() && IsEnabled())
	{
		output();//funcs called in- will get dist and therefore select target
		aim();//funcs called in- will select terget
		shoot();//funcs called in- will get dist and therefore select target
	}
}
void DoctaEight::OperatorControl(void)
{
	Kill;
	while (IsOperatorControl() && IsEnabled())
	{
		output();//funcs called in- updates cam and therefore gets dist and therefore selects target
		if (copilot.GetRawButton(1))
		{
			aim();//funcs called in- will select target
		}
		if (copilot.GetRawButton(2))
		{
			shoot();
		}
		
		arm.Set(copilot.GetTwist());
		//move simple platform arm
		
		intake.Set(copilot.GetY());

		tardis();//robot drive <-below
	}
}
	
void DoctaEight::output (void)
{
	Kill;
			
	UpdateCamData();
	double approximation;
	//GET INFO FROM values()
	
	getDistance(approximation);
	if (IsAutonomous())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Autonomous");
	else if (IsOperatorControl())
		driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Operator");
	
	if (approximation != -1)
		driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "Distance: %f", (double)approximation);
	else if (aimin)
		driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "Double to 0: %f", CamData.centerX);
	
	
	if (choiceTarget == -1)
		driverOut->Printf(DriverStationLCD::kUser_Line2, 3, "no target");
	else if (choiceTarget > 4)
		driverOut->Printf(DriverStationLCD::kUser_Line2, 3, "too many targets");
	else
	{
		driverOut->Printf(DriverStationLCD::kUser_Line4, 3, "Number of targets: %d", CamData.numTargets);
		driverOut->Printf(DriverStationLCD::kUser_Line5, 4, "Target selected: %d", choiceTarget);
	}
	driverOut->UpdateLCD();
}

START_ROBOT_CLASS(DoctaEight);
