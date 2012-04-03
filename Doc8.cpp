









//TEST CODE-- include RAMP STUFF!!!!!!!!!!! (find with search- RAMP STUFF)
//-- include lights - LIGHT STUFF











//note: when cRIO and camera boot up, camera takes longer time to come online than cRIO, therefore, before enabling, wait until light around the camera's
//convex lense is green; then click 'clear errors' in the diagnostics tab of the DS.  There should be a camera error that will dissapear from the
//'messages' box.  If you did not wait long enough, it will reappear.

//note: exp spd for motors not found; single target distancing not opperational; multiple target distancing needs work, therefore, we will probably not use
//this feature at Madera

#include "Doc8.h"

class Doc8_Official : public SimpleRobot
{
	CANDrive *myCD;
	Lift *myLift;
	Shooter *myShooter;
//	Camera *orientation;
	
	DriverStationLCD *driverOut;
	
	Joystick *pilot, *copilot;
	KinectStick *leftArm, *rightArm;
	
	double autonTop, autonBottom;
	int configuration, cycle;
	bool bArcade, hasCamera, flag;
	
//	RampDescend *myRamp;RAMP STUFF
	
	//HOI
//	Relay *red, *blue;//, *camLED;LIGHT STUFF
	
public:
	Doc8_Official(void)
	{
		GetWatchdog().Kill();

		driverOut = DriverStationLCD::GetInstance();
		
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"init");
		driverOut->UpdateLCD();

		myCD = new CANDrive(CAN_DRIVE_LEFT_FRONT, 							
							CAN_DRIVE_RIGHT_FRONT,
							CAN_DRIVE_LEFT_BACK,
							CAN_DRIVE_RIGHT_BACK);
		
		myLift = new Lift (CAN_LIFT_TOWER);
		
		myShooter = new Shooter(CAN_SHOOTER_TOP,
								CAN_SHOOTER_BOTTOM,
								ENCOUNT_SHOOTER);
		myShooter->SetPID(KP, KI, KD);
		
//		myRamp = new RampDescend(CAN_RAMP_DESCENDER);RAMP STUFF
		
		pilot = new Joystick (JOYSTICK_PILOT_PORT);
		copilot = new Joystick (JOYSTICK_COPILOT_PORT);
		
		//HOI
//		red = new Relay(REDSPIKE,Relay::kBothDirections);LIGHT STUFF
//		red->Set(Relay::kOff);LIGHT STUFF
//		blue = new Relay(BLUESPIKE,Relay::kBothDirections);LIGHT STUFF
//		blue->Set(Relay::kOff);LIGHT STUFF
//		camLED = new Relay(CAMSPIKE,Relay::kBothDirections);
//		camLED->Set(Relay::kOff);
//		camLED->Set(Relay::kForward);
		
		leftArm = new KinectStick(KINECT_LEFT);
		rightArm = new KinectStick(KINECT_RIGHT);
		
		cycle = 0;
		
		flag = false;
		bArcade = false;//initial drive mode
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"finished init");
		driverOut->UpdateLCD();
	}

	void RobotMain(void)
	{
		GetWatchdog().Kill();
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"second");
		driverOut->UpdateLCD();
		while (!IsEnabled())
		{
			hasCamera=(IO.GetDigital(10))?(true):(false);
			if(IO.GetDigital(12))
				configuration = 1;
			else if(IO.GetDigital(16))
				configuration = 2;
			else if(IO.GetDigital(14))
				configuration = 3;
			else
				configuration=3;
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"waiting");
			driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"1:fed 2:rnge 3:go");
			driverOut->PrintfLine(DriverStationLCD::kUser_Line3,"config: %d", configuration);
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"cam: %d", hasCamera);
			driverOut->PrintfLine(DriverStationLCD::kUser_Line5,"notify prog. if need other func");
			driverOut->PrintfLine(DriverStationLCD::kUser_Line6,"in case of IO error, default is 3");
			driverOut->UpdateLCD();
		};
		while (1)
		{
			while(IsEnabled() && IsAutonomous())
			{
				Autonomous();
			}
	//		camLED->Set(Relay::kOff);
			while(!IsEnabled())
			{
				driverOut->Clear();
				driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"waiting");
				driverOut->UpdateLCD();
			}
	
	//		if (hasCamera)
	//			camLED->Set(Relay::kForward);
			while(IsEnabled() && IsOperatorControl())
			{
				OperatorControl();
			}
	//		camLED->Set(Relay::kOff);
	//		red->Set(Relay::kOff);LIGHT STUFF
	//		blue->Set(Relay::kOff);LIGHT STUFF
		}
	}
	
	
	void Autonomous(void)
	{
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"Autonomous config: %d", configuration);
		driverOut->UpdateLCD();

//			if (hasCamera)
//			{
//				camLED->Set(Relay::kForward);
//				driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"Cam Enabled");
//				driverOut->UpdateLCD();
//			}
//			else
//			{
				while (myShooter->GetBottomSpeed() > -1000)
				{
					if (configuration == 1 or configuration == 3)
					{
						myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_CONFIG_ONE,SHOOTER_BOTTOM_SPEED_CONFIG_ONE);
					}
					else if (configuration == 2)
					{
						myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_CONFIG_TWO,SHOOTER_BOTTOM_SPEED_CONFIG_TWO);//speed not found actually therefore this is useless
					}
					
					myShooter->UpdateValues();
	
					driverOut->Clear();
					driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"Autonomous config: %d", configuration);
					driverOut->Printf(DriverStationLCD::kUser_Line2,1,"TopIn: %f", myShooter->GetTopInput());
					driverOut->Printf(DriverStationLCD::kUser_Line4,1,"BotIn: %f", myShooter->GetBottomInput());
					driverOut->Printf(DriverStationLCD::kUser_Line3,1,"TopOut: %f", myShooter->GetTopSpeed());
					driverOut->Printf(DriverStationLCD::kUser_Line5,1,"BotOut: %f", myShooter->GetBottomSpeed());
					driverOut->UpdateLCD();
				}
//			}
		
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"Autonomous config: %d", configuration);
		driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"fire");
		driverOut->UpdateLCD();
		
		myLift->AutonomousLift(1);
		
		if (configuration == 1)
		{
			while (IsAutonomous()){Wait(.05);}//motor update time
		};
		
		Wait(4);
		myLift->AutonomousLift(0);
		myShooter->ChangeSpeed(SHOOTER_SPEED_OFF,SHOOTER_SPEED_OFF);
		myShooter->UpdateValues();
		
		if (configuration == 2)
		{
			while (IsAutonomous()){}
		}
		
		//configuration == 3
		myCD->ArcadeDrive(-.5,0);
		Wait(3.1);
		myCD->ArcadeDrive(0,0);
		
		//put down ramp
		//kinect?
		
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"End of Autonomous");
		driverOut->UpdateLCD();
	}

	void OperatorControl(void)
	{
//			if ((pilot->GetRawButton(9) or copilot->GetRawButton(9)) && !flag)
//			{
//				cycle++;
//				if (cycle == 1)
//				{
//					red->Set(Relay::kForward);
//					blue->Set(Relay::kOff);
//				}
//				else if (cycle == 2)
//				{
//					blue->Set(Relay::kForward);
//					red->Set(Relay::kOff);
//				}
//				else if (cycle == 3)
//				{
//					red->Set(Relay::kOff);
//					blue->Set(Relay::kOff);
//					cycle = 0;
//				}
//				flag =1;
//			}
//			else if (!pilot->GetRawButton(9) and !copilot->GetRawButton(9))
//				flag = 0;LIGHT STUFF(this block)
		
//			if (copilot->GetRawButton(11))
//				camLED->Set(Relay::kForward);
//			else if (copilot->GetRawButton(12))
//				camLED->Set(Relay::kOff);
		
		//SWITCHING TO AND FROM TANK AND ARCADE
		if (pilot->GetRawButton(6))
			bArcade = true;
		else if (pilot->GetRawButton(5))
			bArcade = false;
		
		if (bArcade)
			myCD->ArcadeDrive(pilot);
		else 
			myCD -> TankDrive(pilot);
		
		myShooter->Control(copilot);//leave this in the order it is in
		myShooter->UpdateValues();
		
//			SHOOTING SPEEDS BY BUTTON SELECTION
//			if (!hasCamera or copilot->GetRawButton(9))
//			{
				myLift->Control(copilot);
				
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
//			}
//			else
//			{
//				orientation->RunAll();
//				else if(copilot->GetRawButton(1))//shoot
//				{
//					orientation->velocity;set autonTop and autonBottom with this
//					myShooter->ChangeSpeed(autonTop, autonBottom);//////////////////////////////////////////////////////////////////////////////////
//					myShooter->UpdateValues();
//					Wait(2.0);//can this be shortened?????????????????????????????????????????????????????????????????????????????????????
//					myLift->AutonomousLift(1);
//					Wait(4);
//					myLift->AutonomousLift(0);
//					myShooter->ChangeSpeed(SHOOTER_SPEED_OFF,SHOOTER_SPEED_OFF);
//					myShooter->UpdateValues();
//				}
//			}

//			if (hasCamera)
//			{
//				orientation->RunAll();
//				if(pilot->GetRawButton(3))//turn
//				{
//					myCD -> AutoTankDrive(orientation->centerX);
//				}
//			}
		
		//REVERSED TANK DRIVE AND ARCADE DRIVE (MUST HAVE SELECTION WITH G.R.B. 5 & 6
		if (pilot->GetRawButton(1))
		{
			myCD -> SetReversed(true);
		}
		else if (pilot -> GetRawButton(2))
		{
			myCD -> SetReversed(false);
		}
		
//			myRamp->Control(pilot);RAMP STUFF
		
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"TopShotRPM:%.4f", myShooter->GetTopSpeed());
		driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"BotShotRPM:%.4f", myShooter->GetBottomSpeed());
		driverOut->PrintfLine(DriverStationLCD::kUser_Line3,"TopShotInput: %f", myShooter->GetTopInput());
		driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"BotShotInput: %f", myShooter->GetBottomInput());
//			if (hasCamera)
//			{
//				driverOut->PrintfLine(DriverStationLCD::kUser_Line5,"NumberA'Targets: %f", orientation->numberOfTargets);
//				driverOut->PrintfLine(DriverStationLCD::kUser_Line6,"To Center: %f", orientation->centerX);
//			}
		driverOut->UpdateLCD();
		
	}
};

START_ROBOT_CLASS(Doc8_Official);

