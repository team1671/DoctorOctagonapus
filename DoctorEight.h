/*
 *	Gage Ervin, Emmanuel Lopez, Aaron Ramadan, and Robin Chodbury		Team 1671 
 */


/*
 * COPILOT CONTROL
 *
 * 1 to turn (hold)
 *
 * 2 to shoot
 *
 * trigers will move arm
 *
 * intake is right thumbstick
 * 
 * WHEN SHOOTING ---- IMPORTANT!!!
 * UNLESS AT LEAST ONE TARGET IS IN SITE OR WE GET A COMPASS LATER DO NOT SHOOT
 */

/*
 * PILOT
 *
 * car drive, but turns full forward/back at extreme
 *
 * 1 will switch drive
*/

//to do:	launch code;	restrict particles;	

#include <iostream>
#include "WPILib.h"
#include "nivision.h"
#include <stdlib.h>
//for cams
#include "cmath"
//for distance algorithm

#define IO (DriverStation::GetInstance()->GetEnhancedIO())
//IO for remotes

#define CAMERAHEIGHT 80
//inches -- correct later

AxisCamera &camera = AxisCamera::GetInstance("10.16.71.11");
//output from cameras to driverstation (so we can see it)
ColorImage image(IMAQ_IMAGE_HSL);
//create an image to buffer pics

using namespace std;

class DoctaEight : public SimpleRobot
{
	
	BinaryImage* binImg;
	//make image	
	
	Encoder LTopEnc, LBotEnc;
	//launch system encoders
	
	Joystick pilot, copilot;
	
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	//left and right motors, recieve ball, lift ball to launching system, launch system, platform arm
	
	signed char negate, choiceTarget, distanceTarget, itt, rep;
	//negate for turning drive, choice target target selected, distance target for getting distance, itt for itterations
	
	double firstTarget, secondTarget, thirdTarget, decrement, xCircle;
	//targets (fourth is not one two or three XD), decriment to slow motors as aiming

	bool limitedDistance, cycle;
	//switch between primary and secondary distance tracking based on number of targets
	
public:
	DoctaEight(void):
	//constructs
	pilot(1),
	copilot(2),
	//controller(USB port)
	lefty(2),
	righty(3),
	leftyB(4),
	rightyB(5),
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

	//autonomous and driver control
	void targetSelect(void);
	//selects target
	void aim(void);
	//turns to aim
	double fOfX(double x);//part of algorithm
	double getDistance();
	//finds distance using immage
	void shoot(void);
	//uses formula and distance to set jag percentages
	/*void RainbowDash(void)//pony works like c code braces, like this rainbow.Add<Typegoeshere>(variable)
	{
		Dashboard &rainbow = DriverStation::GetInstance()->GetHighPriorityDashboardPacker();
		rainbow.AddCluster();//displays the target nubmers
			rainbow.AddDouble(firstTarget);
			rainbow.AddDouble(secondTarget);
			rainbow.AddDouble(thirdTarget);
		rainbow.FinalizeCluster();
		rainbow.AddCluster();						/////displays the distance from the target, and
			rainbow.AddDouble(getDistance());			//and the diffrence number for angling
			rainbow.AddDouble(decrement);
		rainbow.FinalizeCluster();
		rainbow.Finalize();//need this for the ending
	}*/
	
	void tardis(void)
	{
		if (pilot.GetRawButton(1) && cycle == 0)
		{
			negate *= -1;
			cycle = 1;
		}
		else if (!pilot.GetRawButton(1))
			cycle = 0;
		//to reverse drive
		
		
		if (pilot.GetY() > 0)
		{
			lefty.Set((pilot.GetY() + pilot.GetZ()*2 )*negate);
			leftyB.Set((pilot.GetY() + pilot.GetZ()*2 )*negate);
			righty.Set((pilot.GetY() - pilot.GetZ()*2 )*negate);
			rightyB.Set((pilot.GetY() - pilot.GetZ()*2 )*negate);
		}
		else if (pilot.GetY() < 0)
		{
			lefty.Set((pilot.GetY() - pilot.GetZ()*2 )*negate);
			leftyB.Set((pilot.GetY() - pilot.GetZ()*2 )*negate);
			righty.Set((pilot.GetY() + pilot.GetZ()*2 )*negate);
			rightyB.Set((pilot.GetY() + pilot.GetZ()*2 )*negate);
		}
		else
		{
			lefty.Set(0);
			leftyB.Set(0);
			righty.Set(0);
			rightyB.Set(0);
			
		}

	
	
	}
	
	void Autonomous(void)
	{
		GetWatchdog().Kill();
		cout << "Hello???  Can anyone hear me?" << endl;
		//self aware joke
		aim();
		cout << "Hello???  Can anyone hear me?" << endl;
		shoot();
		cout << "Hello???  Can anyone hear me?" << endl;
	}
	
	void OperatorControl(void)
	{
		GetWatchdog().Kill();
		
		while (IsOperatorControl())
		{
			cout << "Operator\n" ;

			//RainbowDash();//output to dashboard
			
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
};