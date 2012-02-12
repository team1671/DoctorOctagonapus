#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Joystick stick; // only joystick
	SmartDashboard *rainbow;
	
public:
	RobotDemo(void):
		stick(1)		// as they are declared above.
	{
		GetWatchdog().Kill();
		rainbow=SmartDashboard::GetInstance();
		rainbow->init();
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		GetWatchdog().Kill();
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		while (IsOperatorControl())
		{
			GetWatchdog().Kill();
			rainbow->Log(5,"NEEEEEEEEEEEEP");
			Wait(0.005);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

