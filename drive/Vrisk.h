#include "WPILib.h"
#include "KinectStick.h"
#define Kill GetWatchdog().Kill();

#define kVOLTAGE 12.0
#define RAMP 0
#define MAX 2750
#define KP 0.250//proportion for jags
#define KI 0.015//integral change in proportion
#define KD 0.020//damper
#define kENCCOUNT 360

//#define dsio DriverStation::GetInstance()->GetEnhancedIO()

class DoctaEight : public SimpleRobot
{
	Joystick pilot, copilot;
	//	KinectStick *leftArm, *rightArm;
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	signed char negate, drive;
	long signed int bigArse;
	bool limitedDistance, cycle, flag, rotes;
	
	
public:
///////////////////////CORE
		 DoctaEight(void);
	void Autonomous(void);
	void OperatorControl(void);
	void output(void);
///////////////////////DRIVE
	void tardis(void);
	void leftyrighty(double left, double right);//set drive motors on either side
};
