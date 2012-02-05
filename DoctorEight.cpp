
/*
 *  Gage Ervin, Emmanuel Lopez, Aaron Ramadan, and Robin Chodbury		Team 1671 
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

static AxisCamera &camera = AxisCamera::GetInstance("10.16.71.11");
//output from cameras to driverstation (so we can see it)
static ColorImage image(IMAQ_IMAGE_HSL);
//create an image to buffer pics

class DoctaEight : public SimpleRobot
{
	DriverStationLCD* driverOut;
	
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
	DoctaEight(void);
	//autonomous and driver control
	void tardis(void);
	void shoot(void);
	//uses formula and distance to set jag percentages
	void targetSelect(void);
	//selects target
	void aim(void);
	//turns to aim
	double fOfX(double x);//part of algorithm
	double getDistance();
	void OperatorControl(void);
	void Autonomous(void);
	//finds distance using immage
};
