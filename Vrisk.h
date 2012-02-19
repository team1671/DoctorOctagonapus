
/*

find auton value for knocking balls down

find trim value for calculated distance to real distance
dist exp and consider spacial shot exp

comment
 
comment here for controls

set camera to allow anonymous viewing to outline target(available in the web console)
http://firstforge.wpi.edu/integratio...02&view=markup
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
#define RAMP 3
#define MAX 2750
#define KP 0.250//proportion for jags
#define KI 0.015//integral change in proportion
#define KD 0.020//damper
#define kENCCOUNT 360

#define RL 0
#define RH 145
#define GL 0
#define GH 255
#define BL 184
#define BH 255

#define LCRIT 0
#define HCRIT 90

#define BS 198
#define CT 69
#define GA 4

//#define RPMLow 1000 next to

static AxisCamera &eyeOfSauron = AxisCamera::GetInstance("10.16.71.11");
	
//#define dsio DriverStation::GetInstance()->GetEnhancedIO()
//example of use (config in DS)
//		double ptop = (dsio.GetAnalogInRatio(1) - 0.5) * MAX;
//		double pbot = (dsio.GetAnalogInRatio(2) - 0.5) * MAX;
//		LTop.Set(ptop);
//		LBot.Set(pbot);

class DoctaEight : public SimpleRobot
{
	Joystick pilot, copilot;
	KinectStick *leftArm, *rightArm;
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
