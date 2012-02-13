/*
 * TODO LIST
 * 
 * IN aim() use PID to set cam FPS
 * 
 * RGB VALUES ALWAYS
 * 
 * complete shooter (exp equ)
 * 
 * set camera to allow anonymous viewing to outline target(available in the web console)
 * http://firstforge.wpi.edu/integratio...02&view=markup
 * 
 * cleanup highly messy code, break back into sepparate files, definitions, nonglobal vars
 */

/*
 * COPILOT CONTROL
 *
 * A to turn (hold)
 *
 * B to shoot
 * 
 * Y to switch drive  <-not in right now
 *
 * trigers will move arm
 *
 * intake is left thumbstick
 * 
 * WHEN SHOOTING ---- IMPORTANT!!!
 * UNLESS AT LEAST ONE TARGET IS IN SITE OR WE GET A COMPASS LATER DO NOT AIM OR SHOOT BECAUSE NOTHING WILL HAPPEN
 */


/*
 * PILOT
 *
 * press 4 to switch between modified arcade, kinect, and tank(fails)
 *
 * 1 will switch drive
*/
/*
 * The functions have been place into seperate files based on their functionality
 * It says which ones have been move below in the function declarations
 */


#include "WPILib.h"
#include "KinectStick.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "cmath"

#define Kill GetWatchdog().Kill();
#define CAMERAHEIGHT 65
#define ANGLEOFLAUNCH 60
#define angle 54
#define RL 200
#define RH 260
#define GL 140
#define GH 210
#define BL 90
#define RAMP 524
#define BH 135
#define MAX 2750
#define KP 0.250
#define KI 0.015
#define KD 0.020
#define ENCCOUNT 360

class DoctaEight : public SimpleRobot
{	
	BinaryImage *thresholdImage;
	BinaryImage *bigObjectsImage;
	BinaryImage *convexHullImage;
	BinaryImage *filteredImage;
	vector<ParticleAnalysisReport> *particles;
	Joystick pilot, copilot;
	//	KinectStick *leftArm, *rightArm;
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	signed char distanceTarget, choiceTarget, negate, drive;
	double firstTarget, secondTarget, thirdTarget;
	bool limitedDistance, cycle, flag, aimin, shootin;
	struct CamDataStruct
	{
		int numTargets;
		double toCenter, 
			centerY, 
				centerX, 
					firstY, 
						secondY, 
							thirdY, 
								fourthY, 
									area, 
										height, 
											distanceYnorm; //stairway to heaven
	}CamData;
public:
///////////////////////CORE
		 DoctaEight(void);
	void Autonomous(void);
	void OperatorControl(void);
	void output(void);
///////////////////////SEGWAY
	void tardis(void);
	void leftyrighty(double left, double right);//set drive motors on either side
///////////////////////COWBOY
	void aim();
	void shoot(void);
///////////////////////SAURON
	void UpdateCamData();
	void targetSelect(void);//pick the target to shoot at (highest visible) then the one to distance with
	void getDistance(double &approximation);//find distance
	double fOfX(double x);//part of accurate distance finding in getDistance()
};
