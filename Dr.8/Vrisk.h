
/*

BOTHER SOM1 ABT LED'S

find trim value for calculated distance to real distance
dist exp and consider spacial shot exp

convert to real code by testing stuff in vision assistant

manual area over area?

nonglobal

comment
 
comment here for controls

IN aim() use PID to set cam FPS

set camera to allow anonymous viewing to outline target(available in the web console)
http://firstforge.wpi.edu/integratio...02&view=markup


remember to SetSetpoint(f) for PID's
PID
Base should speed up with low I and high D so it never quite goes full in
op cont
also a med to low p so there is a curve

PID
base should slow similarly in auton so it stops only oscilating once or
not at all

PID
as turning cam speed should increase in auton while aiming

PID
shooter motors should have low p for acell curve and an I and D to
make oscilation small as possible(less stress on motors)

 */

#include "WPILib.h"
#include "KinectStick.h"
#include "cmath"
#define Kill GetWatchdog().Kill();
#define BOTTOMTARGET 107.5
#define TOPTARGET 31.5
#define CAMERAHEIGHT 65
#define ANGLEOFLAUNCH 60
#define CAMANGLE 54
#define PI 3.14159265358979323846264

#define kVOLTAGE 12.0
#define RAMP .4
#define MAX 2750
#define KP 0.250//proportion for jags
#define KI 0.015//integral change in proportion
#define KD 0.020//damper
#define kENCCOUNT 360

#define HUL 200
#define HUH 260
#define SAL 140
#define SAH 210
#define LUL 90
#define LUH 135
/*
#define BS 1.0f
#define CT 1.0f
#define GA 1.0f
*/

static AxisCamera &eyeOfSauron = AxisCamera::GetInstance("10.16.71.11");
	
class DoctaEight : public SimpleRobot
{
	Joystick pilot, copilot;
	//	KinectStick *leftArm, *rightArm;
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	signed char distanceTarget, choiceTarget, negate, drive;
	double firstTarget, secondTarget, thirdTarget;
	bool limitedDistance, cycle, flag, aimin, shootin;
	struct CamDataStruct
	{
		int numTargets;
		unsigned int HP, LP;
		double toCenter, 
			centerY, 
				centerX,
					approx,
						area, 
							height, 
								distanceYnorm;//stairway to heaven
	}CamData;
public:
///////////////////////CORE
		 DoctaEight(void);
	void Autonomous(void);
	void OperatorControl(void);
	void output(void);
///////////////////////DRIVE
	void tardis(void);
	void leftyrighty(double left, double right);//set drive motors on either side
///////////////////////EVERYTHING IS AUTOMATED!
	void aim();
	void shoot(void);
///////////////////////SAURON
	void UpdateCamData();
	void setCam(void);
	void targetSelect(void);//pick the target to shoot at (highest visible) then the one to distance with
	double getDistance(double &approximation);//find distance
	double fOfX(double x);//part of accurate distance finding in getDistance()
};
