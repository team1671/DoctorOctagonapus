/*
 * PID not in yet
 * 
 * pilot->y cycles drive: mod arcade/tank/kinect
 * 
 * copilot
 * .3,.4,.5,.6,.7,.8,.9
 * press LB and a,b,x,y,back,start,left stick button, right stick button to set top
 * press RB and a,b,x,y,back,start,left stick button, right stick button to set bot
 * RT will increase overall spd for launch
 * LT will decrease = =
 * intake is y on left thumb
 * arm is X on right thumb
 * 
 */



//cube input
//ADD 524 RAMP    for accel curve in drive




#include "WPILib.h"
//#include "KinectStick.h"


#define MAX 2750
#define KP 0.250
#define KI 0.015
#define KD 0.020
#define ENCCOUNT 360

class DoctaEight : public SimpleRobot
{	
	DriverStationLCD* driverOut;
	//launch system encoders
	
	Timer clock;
	
	Joystick pilot, copilot;
//	KinectStick *leftArm, *rightArm;
	
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	//left and right motors, recieve ball, lift ball to launching system, launch system, platform arm

	signed char negate, choiceTarget, distanceTarget, drive;
	//negate for turning drive, choice target target selected, distance target for getting distance, itt for itterations
	
	double change;
	
	bool limitedDistance, cycle;
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
	//encoders(AChannel, BChannel)
	{
		GetWatchdog().Kill();
		
		driverOut  = DriverStationLCD::GetInstance();

//		leftArm = new KinectStick(1);
//		rightArm = new KinectStick(2);
				
		
		
		limitedDistance = 0;
		cycle = 0;
		drive=1;
		negate=1;
		change= 0;

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
		//CANJags currently % (-1 to 1)
		LBot.SetPID(KP,KI,KD);
		LTop.SetPID(KP,KI,KD);

		//start encoders
	}
	
	void DoctaEight::Autonomous(void)
	{
		GetWatchdog().Kill();
		output();
	}
	
	void DoctaEight::OperatorControl(void)
	{
		GetWatchdog().Kill();
		clock.Reset();
		clock.Start();
		while (IsOperatorControl())
		{
			output();
			
			shoot();
			
			arm.Set(copilot.GetTwist());
			//move simple platform arm
			
			intake.Set(copilot.GetY());

			tardis();//robot drive <-below
			
		}
		clock.Stop();
		//stops encoders
	}

	void shoot(void)
	{
		if ((int)clock.Get()%2 == 1)
		{
			if (copilot.GetZ()>.2)
				change+=1;
			if (copilot.GetZ()<.2)
					change-=1;
		}
		
		if (copilot.GetRawButton(1))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(0);
			if (copilot.GetRawButton(10))
				LBot.Set(0);
		}
		else if (copilot.GetRawButton(2))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(.3*MAX+change);
			if (copilot.GetRawButton(10))
				LBot.Set(.3*MAX+change);
		}
		else if (copilot.GetRawButton(3))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(.4*MAX+change);
			if (copilot.GetRawButton(10))
				LBot.Set(.4*MAX+change);
		}
		else if (copilot.GetRawButton(4))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(.5*MAX+change);
			if (copilot.GetRawButton(10))
				LBot.Set(.5*MAX+change);
		}
		else if (copilot.GetRawButton(5))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(.6*MAX+change);
			if (copilot.GetRawButton(10))
				LBot.Set(.6*MAX+change);
		}
		else if (copilot.GetRawButton(6))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(.7*MAX+change);
			if (copilot.GetRawButton(10))
				LBot.Set(.7*MAX+change);
		}
		else if (copilot.GetRawButton(7))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(.8*MAX+change);
			if (copilot.GetRawButton(10))
				LBot.Set(.8*MAX+change);
		}
		else if (copilot.GetRawButton(8))
		{
			if (copilot.GetRawButton(9))
				LTop.Set(.9*MAX+change);
			if (copilot.GetRawButton(10))
				LBot.Set(.9*MAX+change);
		}
		
	}
	void output (void)
	{
		if (IsAutonomous())
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "Auto");
		else if (IsOperatorControl())
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "Opp");
		driverOut->UpdateLCD();
	}//nom nom nom
	
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
			drive = drive%4 + 2;//	Change to plus 1 for kinect drive be included
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
			//leftyrighty (leftArm -> GetY(), rightArm -> GetTwist());
		}
		else if (drive == 3)
		{
			leftyrighty(negate*pilot.GetY(), negate*pilot.GetRawAxis(4));
		}
	}

};
START_ROBOT_CLASS(DoctaEight); //DOCTOR OCTAGONAPUS WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
