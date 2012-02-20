#include "Doc8.h"

class Doc8_Official : public SimpleRobot
{	
	CANDrive *myCD;
	Lift *myLift;
	Shooter *myShooter;
	
	DriverStationLCD *driverOut;
	
	Joystick *pilot, *copilot;
	KinectStick *leftArm, *rightArm;
	
	bool bArcade;
	
	RampDescend *myRamp;
	
public:
	Doc8_Official(void)
	{
		GetWatchdog().Kill();
		//CONSTRUCTOR
		myCD = new CANDrive(CAN_DRIVE_LEFT_FRONT, 							
							CAN_DRIVE_RIGHT_FRONT,
							CAN_DRIVE_LEFT_BACK,
							CAN_DRIVE_RIGHT_BACK);
		
		myLift = new Lift (CAN_LIFT_TOWER);
		
		myShooter = new Shooter(CAN_SHOOTER_TOP,
								CAN_SHOOTER_BOTTOM,
								ENCOUNT_SHOOTER);
		myShooter->SetPID(KP, KI, KD);
		
		myRamp = new RampDescend(CAN_RAMP_DESCENDER);
		
		driverOut = DriverStationLCD::GetInstance();
		
		pilot = new Joystick (JOYSTICK_PILOT_PORT);
		copilot = new Joystick (JOYSTICK_COPILOT_PORT);
		
		leftArm = new KinectStick(1);
		rightArm = new KinectStick(2);
		
		bArcade = false;
		
	}


	void Autonomous(void)
	{
		GetWatchdog().Kill();
		while(IsEnabled() && IsAutonomous())
		{
			myCD->SpeedTest();
		}
	}

	void OperatorControl(void)
	{
		GetWatchdog().Kill();
		while(IsEnabled() && IsOperatorControl())
		{			
			//SHOOTING SPEEDS BY BUTTON SELECTION
			if(copilot->GetRawButton(1))
			{
				myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_MED,SHOOTER_BOTTOM_SPEED_MED);
			}
			else if(copilot->GetRawButton(2))
			{
				myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_HIGH,SHOOTER_BOTTOM_SPEED_HIGH);
			}
			else if(copilot->GetRawButton(3))
			{
				myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_LOW,SHOOTER_BOTTOM_SPEED_LOW);
			}
			else if (copilot->GetRawButton(8))
			{
				myShooter->ChangeSpeed(SHOOTER_SPEED_OFF,SHOOTER_BOTTOM_SPEED_IDLE);
			}
			else if(copilot->GetRawButton(4))
			{
				myShooter->ChangeSpeed(SHOOTER_SPEED_OFF,SHOOTER_SPEED_OFF);
			}
			
			//SWITCHING TO AND FROM TANK AND ARCADE
			if (pilot->GetRawButton(6))
			{
				bArcade = true;
			}
			else if (pilot->GetRawButton(5))
			{
				bArcade = false;
			}
			
			if (bArcade)
			{
				myCD->ArcadeDrive(pilot);
			}
			else 
			{	
				myCD -> TankDrive(pilot);
			}
			
			//REVERSED TANK DRIVE AND ARCADE DRIVE (MUST HAVE SELECTION WITH G.R.B. 5 & 6
			if (pilot->GetRawButton(1))
			{
				myCD -> SetReversed(true);
			}
			else if (pilot -> GetRawButton(2))
			{
				myCD -> SetReversed(false);
			}
			
			
			
			myLift->Control(copilot);
			myShooter->Control(copilot);
			myShooter->UpdateValues();
			myRamp->Control(pilot);
			
			float lf, lb, rf, rb;
			
			myCD->GetValues(lf, lb, rf, rb);
			
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"TopShotRPM:%.4f", myShooter->GetTopSpeed());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"BotShotRPM:%.4f", myShooter->GetBottomSpeed());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line3,"TopShotInput: %f", myShooter->GetTopInput());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"BotShotInput: %f", myShooter->GetBottomInput());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line5,"Front, %f, %f", lf, rf);
			driverOut->PrintfLine(DriverStationLCD::kUser_Line6,"Back, %f, %f", lb, rb);
			driverOut->UpdateLCD();
			
		}
	}
};

START_ROBOT_CLASS(Doc8_Official);

