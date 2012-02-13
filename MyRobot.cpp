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
	LTop(7),
	LBot(6)
	{
		REDRUM;
		driverOut  = DriverStationLCD::GetInstance();
		
		lefty.ChangeControlMode(CANJaguar::kPercentVbus);
		righty.ChangeControlMode(CANJaguar::kPercentVbus);
		leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
		rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
		intake.ChangeControlMode(CANJaguar::kPercentVbus);
		arm.ChangeControlMode(CANJaguar::kPercentVbus);
		lefty.SetVoltageRampRate(0.3);
		leftyB.SetVoltageRampRate(0.3);
		righty.SetVoltageRampRate(0.3);
		rightyB.SetVoltageRampRate(0.3);
		
		LTop.EnableControl();
		LBot.EnableControl();
		
		LTop.ChangeControlMode(CANJaguar::kPercentVbus);
		LBot.ChangeControlMode(CANJaguar::kPercentVbus);
		LBot.ConfigEncoderCodesPerRev(ENCCOUNT);
		LTop.ConfigEncoderCodesPerRev(ENCCOUNT);
		//CANJags currently % (-1 to 1)
		//LBot.SetPID(KP,KI,KD);
		//LTop.SetPID(KP,KI,KD);
		LBot.SetVoltageRampRate(.3);
		LTop.SetVoltageRampRate(.3);

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
//			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "YUNODRIVEBETTER?");
//			driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "%f",(float)LTop.GetSpeed());
//			driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "%f",(float)LBot.GetSpeed());	

			if (copilot.GetRawButton(1))
			{	
				driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "%f",(float)LTop.GetPosition());
				driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "%f",(float)LBot.GetPosition());	
				LTop.Set(.18+copilot.GetZ()*.4);
				LBot.Set(.45+copilot.GetZ()*.4);
			}
			else if (copilot.GetRawButton(2))
			{
				driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "%f",(float)LTop.GetSpeed());
				driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "%f",(float)LBot.GetSpeed());	
				LTop.Set(.3+copilot.GetZ()*.4);
				LBot.Set(.5+copilot.GetZ()*.4);
			}
			else if (copilot.GetRawButton(3))
			{
				driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "%f",(float)LTop.GetSpeed());
				driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "%f",(float)LBot.GetSpeed());	
				LTop.Set(.45+copilot.GetZ()*.1);
				LBot.Set(.7+copilot.GetZ()*.1);
			}
			else if (copilot.GetRawButton(4))
			{
				driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "%f",(float)LTop.GetSpeed());
				driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "%f",(float)LBot.GetSpeed());	
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
		{
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "YUNODRIVEBETTER?");
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
