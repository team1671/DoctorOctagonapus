/*
 *	Gage Ervin and Emmanuel Lopez		Team 1671 
 *	what happened to the other 7?
 *	historical references: Vriska and Doctaoctagonapus
 */


/*
 * COPILOT CONTROL
 * 1 to turn (hold)
 * 2 to shoot
 * trigers will move arm
 * intake is right thumbstick
 * WHEN SHOOTING ---- IMPORTANT!!!  UNLESS AT LEAST ONE TARGET IS IN SITE OR WE GET A COMPASS LATER DO NOT SHOOT
 * 
 * PILOT
 * car drive, but turns full forward/back at extreme
 * 1 will switch drive
*/
 

/*
 * determine lighting values
 *
 * launch enc for speed ratio
 *
 * make warning if out of range
 * 
*/


#include <iostream>
//for console IO
#include "WPILib.h"

//duh
#include "nivision.h"
//for cams
#include "cmath"

#define IO (DriverStation::GetInstance()->GetEnhancedIO())
//IO from driver station

#define CAMERAHEIGHT 80//inches

AxisCamera &camera = AxisCamera::GetInstance();
//output from cameras to driverstation (so we can see it)
ColorImage image(IMAQ_IMAGE_RGB);
//create an image to buffer pics

class DoctaEight : public SimpleRobot
{
	BinaryImage* binImg;
	//make image	
	
	Encoder LTopEnc, LBotEnc;//launch system encoders
	Joystick pilot, copilot;
	
	CANJaguar lefty, righty, leftyB, rightyB, intake, LTop, LBot, arm;//left and right motors, recieve ball, lift ball to launching system, launch system, platform arm
	signed char negate, choiceTarget, distanceTarget, itt;//negate for turning drive
	double firstTarget, secondTarget, thirdTarget, decrement;//calculated distance to target, to slow motors as aiming

	bool limitedDistance;//switch between primary and secondary distance tracking based on number of targets
	
public:
	DoctaEight(void);
	void targetSelect(void);//selects target	
	void aim(void);
	double fOfX(double x);
	double getDistance();
	void shoot(void);
	void drive(void);
	void RainbowDash(void)//pony works like c code braces, like this rainbow.Add<Typegoeshere>(variable)
	{
		Dashboard &rainbow = DriverStation::GetInstance()->GetHighPriorityDashboardPacker();
		rainbow.AddCluster();						//displays the target nubmers
			rainbow.AddDouble(firstTarget);
			rainbow.AddDouble(secondTarget);
			rainbow.AddDouble(thirdTarget);
		rainbow.FinalizeCluster();
		rainbow.AddCluster();						/////displays the distance from the target, and
			rainbow.AddDouble(getDistance());			//and the diffrence number for angling
			rainbow.AddDouble(decrement);
		rainbow.FinalizeCluster();
		rainbow.Finalize();//need this for the ending
	}
	
	void Autonomous(void)
	{
		GetWatchdog().Kill();
		while (IsAutonomous())
		{
			aim();
			shoot();
			//aim and shoot
		}
	}
	
	void OperatorControl(void)
	{
		GetWatchdog().Kill();
		while (IsOperatorControl())
		{
			//RainbowDash();
			
			if (copilot.GetTop())
				arm.Set(-1);
			else if (copilot.GetTrigger())
				arm.Set(1);
			else
				arm.Set(0);
			//move arm
			
			
			if (copilot.GetRawButton(1))
			{
				aim();
				//aim
			}
			else
				decrement = 1;
			//if not aiming and shooting, return motor power
			
			if (copilot.GetRawButton(2))
				shoot();
			
			intake.Set(copilot.GetTwist());
			//take the balls

			
			if (pilot.GetRawButton(1)){negate *= -1;}
			//to reverse drive
			
			drive();//drive system
			
		}
		LTopEnc.Stop();
		LBotEnc.Stop();
		//stops encoders
	}
};

//called by driverstation

#include "BuildAndConstruct.h"

#include "VisionEightFold.h"

#include "Tardis.h"

#include "SpideySense.h"

START_ROBOT_CLASS(DoctaEight);
