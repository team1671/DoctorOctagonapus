/*
 * PID not in yet
 * 
 * pilot->y cycles drive: mod arcade/tank/kinect
 * 
 * copilot->hold 3 and press triggers to set a top motor
 * hold 2 and press triggers to set a bot motor
 * intake is y on left thumb
 * arm is X on right thumb
 * 
 */

#include "WPILib.h"
#include "KinectStick.h"


//#define MAX 2750
//#define KP 0.250
//#define KI 0.015
//#define KD 0.020
//#define ENCCOUNT 360

class DoctaEight : public SimpleRobot
{	
	DriverStationLCD* driverOut;
	//launch system encoders
	
	Joystick pilot, copilot;
	KinectStick *leftArm, *rightArm;
	
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	//left and right motors, recieve ball, lift ball to launching system, launch system, platform arm

	signed char negate, choiceTarget, distanceTarget, drive;
	//negate for turning drive, choice target target selected, distance target for getting distance, itt for itterations

//	Encoder LTopEnc, LBotEnc;
	
	bool limitedDistance, cycle, flag;
	//switch between primary and secondary distance tracking based on number of targets
	
public:
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
//	//jag(CANjag number)
//	LTopEnc(1,2),
//	LBotEnc(3,4)
	//encoders(AChannel, BChannel)
	{
		GetWatchdog().Kill();
		GetWatchdog().~Watchdog();
		GetWatchdog().Kill();
		
		driverOut  = DriverStationLCD::GetInstance();

		leftArm = new KinectStick(1);
		rightArm = new KinectStick(2);
				
		
		limitedDistance = 0;
		flag = 0;
		cycle = 0;
		drive=1;
		negate=1;

		lefty.ChangeControlMode(CANJaguar::kPercentVbus);
		righty.ChangeControlMode(CANJaguar::kPercentVbus);
		leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
		rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
		intake.ChangeControlMode(CANJaguar::kPercentVbus);
		arm.ChangeControlMode(CANJaguar::kPercentVbus);
		
		LTop.ChangeControlMode(CANJaguar::kPercentVbus);
		LBot.ChangeControlMode(CANJaguar::kPercentVbus);
//		LBot.ConfigEncoderCodesPerRev(ENCCOUNT);
//		LTop.ConfigEncoderCodesPerRev(ENCCOUNT);
		//CANJags currently % (-1 to 1)
//		LBot.SetPID(KP,KI,KD);
//		LTop.SetPID(KP,KI,KD);

//		LTopEnc.Reset();
//		LBotEnc.Reset();
//		//reset encoders
//		LTopEnc.Start();
//		LBotEnc.Start();
//		//start encoders
	}
	

	void shoot(void)
	{
		if (copilot.GetRawButton(3))
			LTop.Set(copilot.GetZ());
		else if (copilot.GetRawButton(2))
			LBot.Set(copilot.GetZ());
	}
	
	void DoctaEight::Autonomous(void)
	{
		GetWatchdog().Kill();
		output();
//		LTopEnc.Stop();
//		LBotEnc.Stop();
//		LTopEnc.Reset();
//		LBotEnc.Reset();
		
	}
	void DoctaEight::OperatorControl(void)
	{
		GetWatchdog().Kill();
//		LTopEnc.Start();
//		LBotEnc.Start();

		while (IsOperatorControl())
		{
			GetWatchdog().Kill();
			output();
			
			shoot();
			
			arm.Set(copilot.GetTwist());
			//move simple platform arm
			
			intake.Set(copilot.GetY());

			tardis();//robot drive <-below
			
		}
//		LTopEnc.Stop();
//		LBotEnc.Stop();
//		//stops encoders
	}
	
	void output (void)
	{
		if (IsAutonomous())
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "Auto");
		else if (IsOperatorControl())
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "Opp");
	}
	
	void DoctaEight::leftyrighty(double left, double right)//set drive motors on either side
	{
		GetWatchdog().Kill();
		righty.Set(right);
		rightyB.Set(right);
		lefty.Set(left);
		leftyB.Set(left);
	}
	
	void DoctaEight::tardis(void)
	{
		GetWatchdog().Kill();
		if (pilot.GetRawButton(1) && cycle == 0)
		{
			negate *= -1;
			cycle = 1;
		}
		else if (pilot.GetRawButton(4) && cycle == 0)
		{
			drive = drive%4 + 1;//	GET Y FOR RIGHT THUMB??
			cycle = 1;
		}
		else if (!pilot.GetRawButton(1) && !pilot.GetRawButton(4))
			cycle = 0;
		//to reverse drive			
		
		if (drive == 1)
		{
			if (pilot.GetY() > .01 && pilot.GetTwist() >= 0)
			{
				if (negate == 1)
					leftyrighty(negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
				else
					leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()));
			}
			else if (pilot.GetY() > .01 && pilot.GetTwist() <= 0)
			{
				if (negate ==1)
					leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()));
				else
					leftyrighty(negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
			}		
			else if (pilot.GetY() < -.01 && pilot.GetTwist() >= 0)
			{
				if (negate == 1)
					leftyrighty(negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
				else
					leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()));
			}
			else if (pilot.GetY() < -.01 && pilot.GetTwist() <= 0)
			{
				if (negate == 1)
					leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()));
				else
					leftyrighty(negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
			}
			else
			{
				if (negate == 1)
					leftyrighty(negate*pilot.GetTwist(), negate*(-pilot.GetTwist()));
				else
					leftyrighty(negate*(-pilot.GetTwist()), negate*pilot.GetTwist());
			}
		}
		else if (drive == 2)
		{
			GetWatchdog().Kill();
			leftyrighty (leftArm -> GetY(), rightArm -> GetTwist());
		}
		else if (drive == 3)
		{
			leftyrighty(negate*pilot.GetY(), negate*pilot.GetRawAxis(4));
		}
	}

};
START_ROBOT_CLASS(DoctaEight);
