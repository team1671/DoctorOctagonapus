//	see	doc8.h	for	todo


#include "DoctorEight.h"

DoctaEight::DoctaEight(void):
//constructs
copilot(2)
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
}

void DoctaEight::Autonomous(void)
{
	GetWatchdog().Kill();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "auto start");
	driverOut->UpdateLCD();
	aim();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "auto mid");
	driverOut->UpdateLCD();
	shoot();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "auto end");
	driverOut->UpdateLCD();
}

void DoctaEight::OperatorControl(void)
{
	GetWatchdog().Kill();
	driverOut->Clear();
	driverOut->UpdateLCD();
	
	while (IsOperatorControl())
	{

		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "operator");
		driverOut->UpdateLCD();
		
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
	}
}

START_ROBOT_CLASS(DoctaEight);
