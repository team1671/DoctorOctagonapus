#include "DoctorEight.h"

DoctaEight::DoctaEight(void):
//constructs
pilot(1),
copilot(2),
//controller(USB port)

lefty(4),
leftyB(5),
righty(2),
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
	negate=1;
	drive=1;
	
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
	if (pilot.GetRawButton(4) && cycle == 0)
	{
		drive *= -1;
		cycle = 1;
	}
	else if (!pilot.GetRawButton(1))
		cycle = 0;
	//to reverse drive			
	
	if (drive == 1)
	{
		if (pilot.GetY() > 0 && pilot.GetZ() >= 0)
		{
			righty.Set(negate*(pilot.GetY() - pilot.GetZ()*2*pilot.GetY()));
			rightyB.Set(negate*(pilot.GetY() - pilot.GetZ()*2*pilot.GetY()));
			lefty.Set(negate*pilot.GetY());
			leftyB.Set(negate*pilot.GetY());
		}
		else if (pilot.GetY() > 0 && pilot.GetZ() <= 0)
		{
			righty.Set(negate*pilot.GetY());
			rightyB.Set(negate*pilot.GetY());
			lefty.Set(negate*(pilot.GetY() + pilot.GetZ()*2*pilot.GetY()));
			leftyB.Set(negate*(pilot.GetY() + pilot.GetZ()*2*pilot.GetY()));
		}		
		else if (pilot.GetY() < 0 && pilot.GetZ() >= 0)
		{
			righty.Set(negate*(pilot.GetY() - pilot.GetZ()*2*pilot.GetY()));
			rightyB.Set(negate*(pilot.GetY() - pilot.GetZ()*2*pilot.GetY()));
			lefty.Set(negate*pilot.GetY());
			leftyB.Set(negate*pilot.GetY());
		}
		else if (pilot.GetY() < 0 && pilot.GetZ() <= 0)
		{
			righty.Set(negate*pilot.GetY());
			rightyB.Set(negate*pilot.GetY());
			lefty.Set(negate*(pilot.GetY() + pilot.GetZ()*2*pilot.GetY()));
			leftyB.Set(negate*(pilot.GetY() + pilot.GetZ()*2*pilot.GetY()));
		}
		else
		{
			righty.Set(0);
			rightyB.Set(0);
			lefty.Set(0);
			leftyB.Set(0);
		}
	}
	else if (drive == -1)
	{
		righty.Set(negate*pilot.GetTwist());
		rightyB.Set(negate*pilot.GetTwist());
		lefty.Set(negate*pilot.GetZ());
		leftyB.Set(negate*pilot.GetZ());
	}
}

void DoctaEight::Autonomous(void)
{
	GetWatchdog().Kill();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "auto start");
	driverOut->UpdateLCD();
	aim();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "auto mid");
	driverOut->UpdateLCD();
	shoot();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "auto end");
	driverOut->UpdateLCD();
}

void DoctaEight::OperatorControl(void)
{
	GetWatchdog().Kill();
	
	while (IsOperatorControl())
	{

	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "operator");
	driverOut->UpdateLCD();
		
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

START_ROBOT_CLASS(DoctaEight);
