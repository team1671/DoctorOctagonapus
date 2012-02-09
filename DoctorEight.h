//PID TO STOP AIMING LOOP IN AUTONOMOUS
//ALSO MUST USE PID TO SLOW AS TURNING TO AIM

//set camera to allow anonymous viewing to outline target(available in the web console)
//EX:	http://firstforge.wpi.edu/integration/viewcvs/viewcvs.cgi/trunk/extensions/camera/SquareTracker/src/edu/wpi/first/wpilibj/examples/SquareTrackerExtension.java?root=smart_dashboard&system=exsy1002&view=markup


//should I make a particle threshold?
//http://www.chiefdelphi.com/forums/showthread.php?t=101878

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
static DriverStationLCD* driverOut  = DriverStationLCD::GetInstance();

class DoctaEight : public SimpleRobot
{
	
	BinaryImage* binImg;
	//make image
	
	Joystick copilot;
	signed char choiceTarget, distanceTarget;
	//negate for turning drive, choice target target selected, distance target for getting distance, itt for itterations
	
	double firstTarget, secondTarget, thirdTarget, xCircle;
	//targets (fourth is not one two or three XD), decriment to slow motors as aiming

	bool limitedDistance;
	//switch between primary and secondary distance tracking based on number of targets
	
public:
	DoctaEight(void);
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
