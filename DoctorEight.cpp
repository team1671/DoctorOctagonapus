#include "DoctorEight.h"

DoctaEight::DoctaEight(void):
//constructs
driverOut(DriverStationLCD::GetInstance()),
pilot(1),
copilot(2),
//controller(USB port)
lefty(4),
righty(2),
leftyB(5),
rightyB(3),
arm(9),
intake(8),
LTop(6),
LBot(7),
//jag(CANjag number)
LTopEnc(1,2),
LBotEnc(3,4)
//encoders(AChannel, BChannel)
{
	GetWatchdog().Kill();
	binImg = image.ThresholdHSL(0, 250, 30, 200, 130, 270);
	//HSL values (MUST BE FOUND BY EXPERIMENT)
	camera.WriteMaxFPS(30);
	//FPS
	camera.WriteBrightness(30);
	//
	camera.WriteWhiteBalance(AxisCamera::kWhiteBalance_Hold);
	//white balance -- set manually via internet connection w/hold
	camera.WriteResolution(AxisCamera::kResolution_320x240);
	//resolution
	camera.WriteColorLevel(100);
	//low color
	camera.WriteCompression(30);
	//lower easier on CRIO and harder on cam


	limitedDistance = 0;
	cycle = 0;
	decrement=1;
	negate=1;
	itt = 0;
	rep = 0;
	
	lefty.ChangeControlMode(CANJaguar::kPercentVbus);
	righty.ChangeControlMode(CANJaguar::kPercentVbus);
	leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
	rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
	intake.ChangeControlMode(CANJaguar::kPercentVbus);
	LTop.ChangeControlMode(CANJaguar::kPercentVbus);
	LBot.ChangeControlMode(CANJaguar::kPercentVbus);
	//CANJags currently % (-1 to 1)
	
	LTopEnc.Reset();
	LBotEnc.Reset();
	//reset encoders
	LTopEnc.Start();
	LBotEnc.Start();
	//start encoders
}

void DoctaEight::tardis(void)
{
	if (pilot.GetRawButton(1) && cycle == 0)
	{
		negate *= -1;
		cycle = 1;
	}
	else if (!pilot.GetRawButton(1))
		cycle = 0;
	//to reverse drive			
	righty.Set(-(pilot.GetY() + pilot.GetZ()));
	rightyB.Set(-(pilot.GetY() + pilot.GetZ()));
	lefty.Set(-(pilot.GetY() - pilot.GetZ()));
	leftyB.Set(-(pilot.GetY() - pilot.GetZ()));
}

void DoctaEight::Autonomous(void)
{
	GetWatchdog().Kill();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "auto:");
	aim();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "auto:");
	shoot();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "auto:");
}

void DoctaEight::OperatorControl(void)
{
	GetWatchdog().Kill();
	
	while (IsOperatorControl())
	{
		
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "operator");

		
		if (copilot.GetTop())
			arm.Set(-1);
		else if (copilot.GetTrigger())
			arm.Set(1);
		else
			arm.Set(0);
		//move arm
		
		above:
		//this is because targets may momentarily not be tracked in which case I want to re-execute
		//copilot portion before drive is called and jags are set to 0 or other by pilot
		if (copilot.GetRawButton(1))
		{
			aim();
		}
		else
			decrement = 1;
		//if not aiming and shooting, return motor power
		
		
		if (copilot.GetRawButton(2))
		{
			shoot();
		}
		
		if (copilot.GetRawButton(2) or copilot.GetRawButton(1))
			goto above;
		
		intake.Set(copilot.GetTwist());
		//take the balls

		tardis();		
	}
	LTopEnc.Stop();
	LBotEnc.Stop();
	//stops encoders
}
//printf("Operator\n");

START_ROBOT_CLASS(DoctaEight);
