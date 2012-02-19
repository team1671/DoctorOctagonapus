#include "WPILib.h"
#include "KinectStick.h"
/*
 * config encoder enabled
 * encoder sarted in contructor
 * added defined speeds for shooter/launcher
 */

#define MAX 2700//max speed for jags in rpm
#define kENCCOUNT 360//pulses per rotation from encoder
#define KP 0.250//volts it is allowed to change the jag speed by per unit (was exp determined)
#define KI 0.015//integral PID is allowed to change KP by
#define KD 0.020//change PID uses to kill oscillatio   n as nearing target voltage
#define kVOLTAGE 12//max voltage
#define RAMP .3
#define REDRUM GetWatchdog().Kill()
#define RPMLow 1000 //Speed settings for speed control on shooter/launcher
#define RPMMid 1200
#define RPMHigh 1400


class DoctaEight : public SimpleRobot
{	
	DriverStationLCD* driverOut;
	
	Joystick pilot, copilot;
	
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	
	int outputCounter, RPMoffset, ShooterState, LTopSpeed, LBotSpeed;
	char negate;
	bool FlagB5, FlagB6, cycle;
	
public:
	DoctaEight::DoctaEight(void):
	pilot(1),
	copilot(2),
	
	lefty(4),
	leftyB(5),
	righty(2),
	rightyB(3),
	
	arm(9),
	intake(8),
	LTop(7,CANJaguar::kSpeed),
	LBot(6,CANJaguar::kSpeed)
	{
		REDRUM;
		driverOut  = DriverStationLCD::GetInstance();
		
		
		lefty.ChangeControlMode(CANJaguar::kPercentVbus);
		righty.ChangeControlMode(CANJaguar::kPercentVbus);
		leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
		rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
		intake.ChangeControlMode(CANJaguar::kPercentVbus);
		arm.ChangeControlMode(CANJaguar::kPercentVbus);

		LTop.ChangeControlMode(CANJaguar::kSpeed);
		LBot.ChangeControlMode(CANJaguar::kSpeed);
		LBot.SetPID(KP,KI,KD);
		LTop.SetPID(KP,KI,KD);
		
		lefty.ConfigMaxOutputVoltage(kVOLTAGE);
		leftyB.ConfigMaxOutputVoltage(kVOLTAGE);
		righty.ConfigMaxOutputVoltage(kVOLTAGE);
		rightyB.ConfigMaxOutputVoltage(kVOLTAGE);
		LTop.ConfigMaxOutputVoltage(kVOLTAGE);
		LBot.ConfigMaxOutputVoltage(kVOLTAGE);
		
		LBot.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		LTop.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		LBot.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		LTop.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		//WHY BOTH?
		
		LBot.ConfigEncoderCodesPerRev(kENCCOUNT);
		LTop.ConfigEncoderCodesPerRev(kENCCOUNT);
		LTop.SetVoltageRampRate(RAMP);//MAXIMUM
		LBot.SetVoltageRampRate(RAMP);//V CHANGE
										//OR I + P
		lefty.SetVoltageRampRate(RAMP);
		leftyB.SetVoltageRampRate(RAMP);
		righty.SetVoltageRampRate(RAMP);
		rightyB.SetVoltageRampRate(RAMP);
		LTop.EnableControl();
		LBot.EnableControl();
		rightyB.EnableControl();
		righty.EnableControl();
		leftyB.EnableControl();
		lefty.EnableControl();
		
		negate=1;
		outputCounter = 0;
		RPMoffset = 0;
		ShooterState = 0;
		LTopSpeed = 0;
		LBotSpeed = 0;
		
		FlagB5 = 0;
		FlagB6 = 0;

	}
	
	void DoctaEight::Autonomous(void)
	{
		REDRUM;
		
		output();
	}
	
	void DoctaEight::OperatorControl(void)
	{
		REDRUM;
		LTop.SetSafetyEnabled(0);
		LTop.EnableControl();
		LBot.SetSafetyEnabled(0);
		LBot.EnableControl();
		while (IsOperatorControl())
		{
			REDRUM;
			output();
			if (pilot.GetRawButton(6))
			{
				arm.Set (.3);
			}
			
			else if (pilot.GetRawButton(5))
			{
				
				arm.Set (-.3);
			}
			else
				arm.Set (0);
				
		
			//Set Shooter state and reset RPMoffset if necessary
			
			if (copilot.GetRawButton(1)) //BUTTON A
			{	
				REDRUM;
				if(ShooterState != 1) 
				{
					REDRUM;
					ShooterState = 1;	
					RPMoffset = 0;
				}
			}
			else if (copilot.GetRawButton(2)) //BUTTON B
			{
				REDRUM;
				if(ShooterState != 2) 
				{
					REDRUM;
					ShooterState = 2;	
					RPMoffset = 0;
				}
			}
			else if (copilot.GetRawButton(3)) //BUTTON X
			{
				REDRUM;
				if(ShooterState != 3) 
				{
					REDRUM;
					ShooterState = 3;	
					RPMoffset = 0;
				}
			}
			else if (copilot.GetRawButton(4)) //BUTTON Y
			{
				REDRUM;
				if(ShooterState != 4) 
					{
						REDRUM;
						ShooterState = 4;	
						RPMoffset = 0;
					}
			}
			
			//increment or decrement RPMoffset
			if(copilot.GetRawButton(5)) //BUTTON LB
			{
				REDRUM;
				FlagB5 = true;
			}
			else if(copilot.GetRawButton(6)) //BUTTON RB
			{
				REDRUM;
				FlagB6 = true;
			}
			
			if(FlagB5 == true && copilot.GetRawButton(5) == false)
			{
				REDRUM;
				RPMoffset -= 50;
				FlagB5 = false;
			}
			else if(FlagB6 && !copilot.GetRawButton(6))
			{
				REDRUM;
				RPMoffset += 50;
				FlagB6 = false;
			}
			
			if (pilot.GetRawButton(1) && !cycle)
			{
				cycle = 1;
				negate*=-1;
			}
			else
			{
				cycle=0;
			}
			
			//prepare shooter/launcher ouput speed
			if(ShooterState == 1) { 
				REDRUM;		//BUTTON A
				LTopSpeed = MAX/12;
				LBotSpeed = RPMMid + RPMoffset;
			}
			else if(ShooterState == 2) { //BUTTON B
				REDRUM;
				LTopSpeed = MAX/12;
				LBotSpeed = RPMLow + RPMoffset;
			}
			else if(ShooterState == 3) { //BUTTON X
				REDRUM;
				LTopSpeed = MAX/12;
				LBotSpeed = RPMHigh + RPMoffset;
			}
			else if(ShooterState == 4) { //BUTTON Y
				REDRUM;
				LTopSpeed = 0;
				LBotSpeed = 0;
			}
			
			//set shooter launcher speed to CANJags                                                  
			LTop.Set(LTopSpeed);
			LBot.Set(-LBotSpeed);
			
			
			//move simple platform arm	
			leftyrighty(-pilot.GetY(), -pilot.GetRawAxis(4));
			
			
				
			
			intake.Set(-copilot.GetY());
			
			
		}
		//stops encoders
	}

	
	void output (void)
	{
		REDRUM;
		if (IsAutonomous())
			driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "blaarag");
		else if (IsOperatorControl())
		{	
			REDRUM;
		}
		outputCounter++;
					
					if (outputCounter % 30 == 0){
						REDRUM;
					driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "Top Shooter RPM: %f",(float)LTop.GetSpeed());
					driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "Bot Shooter RPM: %f",(float)LBot.GetSpeed());
			//		driverOut->PrintfLine(DriverStationLCD::kUser_Line6, "Pilot Z-Axis: %f",pilot.GetZ());
					
					}
					
					if (outputCounter % 60 == 0){
						REDRUM;
					driverOut->PrintfLine(DriverStationLCD::kUser_Line4, "Top CANJag Temp: %f Celcius",LTop.GetTemperature()*(9/5) + 32);
					driverOut->PrintfLine(DriverStationLCD::kUser_Line5, "Bot CANJag Temp: %f Celcius",LBot.GetTemperature()*(9/5) + 32);
				    outputCounter = 1;
					}
		driverOut->UpdateLCD();
	}//nom nom nom
	
	void DoctaEight::leftyrighty(double left, double right)//set drive motors on either side
	{
		REDRUM;
		righty.Set(negate*right*.9);
		rightyB.Set(negate*right*.9);
		lefty.Set(negate*left*.9);
		leftyB.Set(negate*left*.9);
	}

};
START_ROBOT_CLASS(DoctaEight); //DOCTOR OCTAGONAPUS WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO; we need to write out the analogies for comp
