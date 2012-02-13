#include "WPILib.h"
//#include "KinectStick.h"
/*
 * config encoder enabled
 * encoder sarted in contructor
 */

#define MAX 50
#define KP 0.250
#define KI 0.015
#define KD 0.020
#define ENCCOUNT 360
#define REDRUM GetWatchdog().Kill()

class DoctaEight : public SimpleRobot
{	
	DriverStationLCD* driverOut;
	
	Joystick pilot, copilot;
	
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	
	int outputCounter;
	
public:
	DoctaEight::DoctaEight(void):
	pilot(1),
	copilot(2),
	
	lefty(4),
	leftyB(5),
	righty(2),
	rightyB(3),
	
	arm(9),
	intake(8),
	LTop(7,CANJaguar::kSpeed),
	LBot(6,CANJaguar::kSpeed)
	{
		REDRUM;
		driverOut  = DriverStationLCD::GetInstance();
		
		lefty.ChangeControlMode(CANJaguar::kPercentVbus);
		righty.ChangeControlMode(CANJaguar::kPercentVbus);
		leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
		rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
		intake.ChangeControlMode(CANJaguar::kPercentVbus);
		arm.ChangeControlMode(CANJaguar::kSpeed);
//		lefty.SetVoltageRampRate(0.3);
//		leftyB.SetVoltageRampRate(0.3);
//		righty.SetVoltageRampRate(0.3);
//		rightyB.SetVoltageRampRate(0.3);
		
		LTop.ChangeControlMode(CANJaguar::kSpeed);
		LBot.ChangeControlMode(CANJaguar::kSpeed);
		LBot.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		LTop.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		LBot.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		LTop.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		LBot.ConfigEncoderCodesPerRev(ENCCOUNT);
		LTop.ConfigEncoderCodesPerRev(ENCCOUNT);
		LTop.EnableControl();
		LBot.EnableControl();
		//CANJags currently % (-1 to 1)
		LBot.SetPID(KP,KI,KD);
		LTop.SetPID(KP,KI,KD);
		LBot.SetVoltageRampRate(.3);
		LTop.SetVoltageRampRate(.3);
		
		outputCounter = 0;

	}
	
	void DoctaEight::Autonomous(void)
	{
		REDRUM;
		output();
	}
	
	void DoctaEight::OperatorControl(void)
	{
		REDRUM;
		LTop.SetSafetyEnabled(0);
		LTop.EnableControl();
		LBot.SetSafetyEnabled(0);
		LBot.EnableControl();
		while (IsOperatorControl())
		{
			REDRUM;
			output();
			
			outputCounter++;
			
			if (outputCounter % 30 == 0){
				REDRUM;
			driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "Top Shooter RPM: %f",(float)LTop.GetSpeed());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "Bot Shooter RPM: %f",(float)LBot.GetSpeed());	
			}
			
			if (outputCounter % 70 == 0){
				REDRUM;
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4, "Top CANJag Temp: %f Celcius",LTop.GetTemperature()*(9/5) + 32);
			driverOut->PrintfLine(DriverStationLCD::kUser_Line5, "Bot CANJag Temp: %f Celcius",LBot.GetTemperature()*(9/5) + 32);	
			}
			
			if (copilot.GetRawButton(1))
			{		
				REDRUM;
//				LTop.Set(.18+copilot.GetZ()*.4);
//				LBot.Set(.45+copilot.GetZ()*.4);
				LTop.Set(1000);
				LBot.Set(1200);
			}
			else if (copilot.GetRawButton(2))
			{
				REDRUM;
				//LTop.Set(.3+copilot.GetZ()*.4);
				//LBot.Set(.5+copilot.GetZ()*.4);
				LTop.Set(800);
				LBot.Set(1000);
			}
			else if (copilot.GetRawButton(3))
			{
				REDRUM;
//				LTop.Set(.45+copilot.GetZ()*.1);
//				LBot.Set(.7+copilot.GetZ()*.1);
				LTop.Set(1200);
				LBot.Set(1400);
			}
			else if (copilot.GetRawButton(4))
			{
				REDRUM;
				LTop.Set(0);
				LBot.Set(0);
			}
			
			//move simple platform arm
			leftyrighty(-pilot.GetY(), -pilot.GetTwist());
			intake.Set(-copilot.GetY());
			
		}
		//stops encoders
	}

	
	void output (void)
	{
		REDRUM;
		if (IsAutonomous())
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "blaarag");
		else if (IsOperatorControl())
		{	REDRUM;
	//		driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "YUNODRIVEBETTER?");
//			driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "%f",(float)LTop.GetSpeed());
//			driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "%f",(float)LBot.GetSpeed());	
		}
		driverOut->UpdateLCD();
	}//nom nom nom
	
	void DoctaEight::leftyrighty(double left, double right)//set drive motors on either side
	{
		REDRUM;
		righty.Set(right*.9);
		rightyB.Set(right*.9);
		lefty.Set(left*.9);
		leftyB.Set(left*.9);
	}

};
START_ROBOT_CLASS(DoctaEight); //DOCTOR OCTAGONAPUS WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
