/*
 *	Gage Ervin and Emmanuel Lopez		Team 1671 
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

//to do:	launch code;	

#include <iostream>
#include "WPILib.h"
#include "nivision.h"
//for cams
#include "cmath"
//for distance algorithm

#define IO (DriverStation::GetInstance()->GetEnhancedIO())
//IO for remotes

#define CAMERAHEIGHT 80
//inches -- correct later

AxisCamera &camera = AxisCamera::GetInstance();
//output from cameras to driverstation (so we can see it)
ColorImage image(IMAQ_IMAGE_RGB);
//create an image to buffer pics

class DoctaEight : public SimpleRobot
{
	BinaryImage* binImg;
	//make image	
	
	Encoder LTopEnc, LBotEnc;
	//launch system encoders
	
	Joystick pilot, copilot;
	
	CANJaguar lefty, righty, leftyB, rightyB, intake, LTop, LBot, arm;
	//left and right motors, recieve ball, lift ball to launching system, launch system, platform arm
	
	signed char negate, choiceTarget, distanceTarget, itt;
	//negate for turning drive, choice target target selected, distance target for getting distance, itt for itterations
	
	double firstTarget, secondTarget, thirdTarget, decrement;
	//targets (fourth is not one two or three XD), decriment to slow motors as aiming

	bool limitedDistance;
	//switch between primary and secondary distance tracking based on number of targets
	
public:
	DoctaEight(void);
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
	void drive(void);
	//the drive system
	void RainbowDash(void)
	//pony works like c code braces, like this rainbow.Add<Typegoeshere>(variable)
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
		GetWatchdog().Kill();			//PK puppy
		
		cout << "Operator" << endl;
		
		while (IsOperatorControl())
		{
			
			RainbowDash();//output to dashboard
			
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
				cout << "Operator" << endl;
			}
			else
				decrement = 1;
			//if not aiming and shooting, return motor power
			
			
			if (copilot.GetRawButton(2))
			{
				shoot();
				cout << "Operator" << endl;
			}
			
			
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

#include "BuildAndConstruct.h"

#include "SpideySense.h"

#include "VisionEightFold.h"

#include "Tardis.h"

START_ROBOT_CLASS(DoctaEight);
