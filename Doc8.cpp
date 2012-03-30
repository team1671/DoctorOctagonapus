<<<<<<< HEAD
//note: when cRIO and camera boot up, camera takes longer time to come online than cRIO, therefore, before enabling, wait until light around the camera's
//convex lense is green; then click 'clear errors' in the diagnostics tab of the DS.  There should be a camera error that will dissapear from the
//'messages' box.  If you did not wait long enough, it will reappear.

//note: some changes aside from the camera have been made; for removed code, see txt file; compare with older version~ I have one or it can be found in
//GIT's history

//note: exp spd for motors not found; single target distancing not opperational; multiple target distancing needs work, therefore, we will probably not use
//this feature at Davis

#include "Doc8.h"

class Doc8_Official : public SimpleRobot
{
	CANDrive *myCD;
	Lift *myLift;
	Shooter *myShooter;
	Camera *orientation;
	
	DriverStationLCD *driverOut;
	
	Joystick *pilot, *copilot;
	KinectStick *leftArm, *rightArm;
	
	double autonTop, autonBottom;
	int configuration, cycle;
	bool bArcade, hasCamera, flag;
	
	RampDescend *myRamp;
	
	//HOI
	Relay *red, *blue, *camLED;
	
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
		
		//HOI
		red = new Relay(REDSPIKE,Relay::kBothDirections);
		red->Set(Relay::kOff);
		blue = new Relay(BLUESPIKE,Relay::kBothDirections);
		blue->Set(Relay::kOff);
		camLED = new Relay(CAMSPIKE,Relay::kBothDirections);
		camLED->Set(Relay::kOff);
		camLED->Set(Relay::kForward);
		
		leftArm = new KinectStick(KINECT_LEFT);
		rightArm = new KinectStick(KINECT_RIGHT);
		
		cycle = 0;
		
		flag = false;
		bArcade = false;//initial drive mode
		
//		hasCamera=(IO.GetDigital(?))?(false):(true)//Psoc is not WIRED
/*		if(IO.GetDigital(?))
			configuration = 1;
		if(IO.GetDigital(?))
			configuration = 2;
		if(IO.GetDigital(?))
			configuration = 3;*/
		configuration = 55;//1norm, 2 range, 55 lasting, 56 move
		hasCamera = false;
	}


	void Autonomous(void)
	{
		GetWatchdog().Kill();
		while(IsEnabled() && IsAutonomous())//this is kinda ghetto but..
		{
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"Autonomous");

//			if (hasCamera)//included!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//			{
//				camLED->Set(Relay::kForward);
//				driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"HAS CAMERA!!!!!!!!!!!!!!");
//				driverOut->UpdateLCD();
//			}
//			else
//			{
//				if (configuration == 1 or configuration == 55 or configuration == 56 )
//				{
//					myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_CONFIG_ONE,SHOOTER_BOTTOM_SPEED_CONFIG_ONE);
//					myShooter->UpdateValues();
//				}
//				else if (configuration == 2)
//				{
//					myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_CONFIG_TWO,SHOOTER_BOTTOM_SPEED_CONFIG_TWO);
//					myShooter->UpdateValues();
//				}
//			}
			while (myShooter->GetBottomSpeed() > -1000)
			{
				myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_CONFIG_ONE,SHOOTER_BOTTOM_SPEED_CONFIG_ONE);
				myShooter->UpdateValues();
			}
			
			driverOut->PrintfLine(DriverStationLCD::kUser_Line5,"TopShotInput: %f", myShooter->GetTopInput());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line6,"BotShotInput: %f", myShooter->GetBottomInput());
			
			driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"fire");
			driverOut->UpdateLCD();
			
			myLift->AutonomousLift(1);
			
			if (configuration == 55)
			{
				while (IsAutonomous()){}
			}
			else{}
			
			Wait(4);
			myLift->AutonomousLift(0);
			myShooter->ChangeSpeed(SHOOTER_SPEED_OFF,SHOOTER_SPEED_OFF);
			myShooter->UpdateValues();
			
			if (configuration == 56)
			{
			myCD->ArcadeDrive(-.5,0);
			Wait(3.1);
			myCD->ArcadeDrive(0,0);
			}
			else{}
			
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"End of Autonomous");
			driverOut->UpdateLCD();
		}
		camLED->Set(Relay::kOff);
	}

	void OperatorControl(void)
	{
		GetWatchdog().Kill();
		if (hasCamera)
			camLED->Set(Relay::kForward);
		while(IsEnabled() && IsOperatorControl())
		{
			if ((pilot->GetRawButton(9) or copilot->GetRawButton(9)) && !flag)
			{
				cycle++;
				if (cycle == 1)
				{
					red->Set(Relay::kForward);
					blue->Set(Relay::kOff);
				}
				else if (cycle == 2)
				{
					blue->Set(Relay::kForward);
					red->Set(Relay::kOff);
				}
				else if (cycle == 3)
				{
					red->Set(Relay::kOff);
					blue->Set(Relay::kOff);
					cycle = 0;
				}
				flag =1;
			}
			else if (!pilot->GetRawButton(9) and !copilot->GetRawButton(9))
				flag = 0;
			
			if (copilot->GetRawButton(11))
				camLED->Set(Relay::kForward);
			else if (copilot->GetRawButton(12))
				camLED->Set(Relay::kOff);
			
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
			
			myShooter->Control(copilot);//leave thisaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaah
			myShooter->UpdateValues();
			
//			SHOOTING SPEEDS BY BUTTON SELECTION
			if (/*!hasCamera or*/ /*this is while not distance shooting*/!/**/copilot->GetRawButton(9))//change!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			{
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
			}
			else
			{
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
			}

			if (hasCamera)
			{
				orientation->RunAll();
				if(pilot->GetRawButton(3))//turn
				{
					myCD -> AutoTankDrive(orientation->centerX);
				}
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
			
			myRamp->Control(pilot);
			
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"TopShotRPM:%.4f", myShooter->GetTopSpeed());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"BotShotRPM:%.4f", myShooter->GetBottomSpeed());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line3,"TopShotInput: %f", myShooter->GetTopInput());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"BotShotInput: %f", myShooter->GetBottomInput());
			if (hasCamera)
			{
				driverOut->PrintfLine(DriverStationLCD::kUser_Line5,"NumberA'Targets: %f", orientation->numberOfTargets);
				driverOut->PrintfLine(DriverStationLCD::kUser_Line6,"To Center: %f", orientation->centerX);
			}
			driverOut->UpdateLCD();
			
		}
		camLED->Set(Relay::kOff);
		red->Set(Relay::kOff);
		blue->Set(Relay::kOff);
		
	}
};

START_ROBOT_CLASS(Doc8_Official);

=======
//pid for turning
//dist->jag spds
//update cam stuff


#include "Doc8.h"
//#include "skelton.h"

class Doc8_Official : public SimpleRobot
{
	CANDrive *myCD;
	Lift *myLift;
	Shooter *myShooter;
	CATS *orientation;
	
	DriverStationLCD *driverOut;
	
	Joystick *pilot, *copilot;
	KinectStick *leftArm, *rightArm;
	
	int configuration;
	bool bArcade, hasCamera;
	
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
		
		
		
		leftArm = new KinectStick(KINECT_LEFT);
		rightArm = new KinectStick(KINECT_RIGHT);
		
		bArcade = false;//initial drive mode
		
		configuration = 1;//set to pee-sock l8er!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		hasCamera = false;//set to switch l8er!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}


	void Autonomous(void)
	{
		GetWatchdog().Kill();
		while(IsEnabled() && IsAutonomous())//this is kinda ghetto but..
		{
	
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"Autonomous");
			myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_MED,SHOOTER_BOTTOM_SPEED_MED);//why does i does this??????????????????
			Wait(.3);//why does i does this????????????????????????????????????????????????????????????????????????????????
			//removed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//			myShooter->m_canTopShooter->Set(SHOOTER_TOP_SPEED_CONFIG_ONE);
//			myShooter->m_canBottomShooter->Set(SHOOTER_BOTTOM_SPEED_CONFIG_ONE);

			if (hasCamera)//included!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			{
				driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"HAS CAMERA!!!!!!!!!!!!!!");
				driverOut->UpdateLCD();
			}
			else
			{
				if (configuration == 1)
				{
					myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_CONFIG_ONE,SHOOTER_BOTTOM_SPEED_CONFIG_ONE);
				}
				else if (configuration == 2)
				{
					myShooter->ChangeSpeed(SHOOTER_TOP_SPEED_CONFIG_TWO,SHOOTER_BOTTOM_SPEED_CONFIG_TWO);
				}
			}
			
			driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"fire");
			driverOut->UpdateLCD();
			
			Wait(2.0);//can this be shortened?????????????????????????????????????????????????????????????????????????????????????
			myLift->AutonomousLift(1);
			Wait(4);
			myLift->AutonomousLift(0);
			myShooter->ChangeSpeed(SHOOTER_SPEED_OFF,SHOOTER_SPEED_OFF);
			myShooter->UpdateValues();
			
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"End of Autonomous: Beginning Kinect");
			driverOut->UpdateLCD();
			
			
			Kinect://I still want the skeleton!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				myCD->ArcadeDriveKinect(leftArm->GetY());
				myRamp->AutonomousArm(rightArm->GetY());
			if (IsAutonomous() && IsEnabled())
				goto Kinect;
			
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"End of Hybrid");
			driverOut->UpdateLCD();
		}
	}

	void OperatorControl(void)
	{
		GetWatchdog().Kill();
		while(IsEnabled() && IsOperatorControl())
		{	
			
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
			
			myShooter->Control(copilot);//leave this
			myShooter->UpdateValues();
			//SHOOTING SPEEDS BY BUTTON SELECTION
			if (!hasCamera or copilot->GetRawButton(9))//change!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			{
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
			}
			else
			{
				orientation->RunAll();
				if(copilot->GetRawButton(2))//turn
				{
					myCD -> AutoTankDrive(orientation->centerX);
				}
				//first fix speed setting w/exp and equ then play with first wait to see if can be shorter
				/*else if(copilot->GetRawButton(1))//shoot
				{
					orientation->approximateDistance;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					myShooter->ChangeSpeed(bwahahahahhahahaha,hahahajfdhslkdjfhsdkj);//////////////////////////////////////////////////////////////////////////////////
					myShooter->UpdateValues();
					Wait(2.0);//can this be shortened?????????????????????????????????????????????????????????????????????????????????????
					myLift->AutonomousLift(1);
					Wait(4);
					myLift->AutonomousLift(0);
					myShooter->ChangeSpeed(SHOOTER_SPEED_OFF,SHOOTER_SPEED_OFF);
					myShooter->UpdateValues();
				}*/
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
			
			myRamp->Control(pilot);
			
			driverOut->Clear();
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1,"TopShotRPM:%.4f", myShooter->GetTopSpeed());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line2,"BotShotRPM:%.4f", myShooter->GetBottomSpeed());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line3,"TopShotInput: %f", myShooter->GetTopInput());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"BotShotInput: %f", myShooter->GetBottomInput());
			driverOut->PrintfLine(DriverStationLCD::kUser_Line4,"NumberA'Targets: %f", orientation->numberOfTargets);
			driverOut->UpdateLCD();
			
		}
	}
};

START_ROBOT_CLASS(Doc8_Official);

>>>>>>> b575175822067cc77622453ead236f74494a7ac7
