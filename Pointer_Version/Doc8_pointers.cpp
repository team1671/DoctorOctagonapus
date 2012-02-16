#include "Doc8_header.h"

class DoctaEight : public SimpleRobot
{
	DriverStationLCD *driverOut;

	Joystick *pilot, *copilot;

	CANJaguar *lefty, *righty, *leftyB, *rightyB, *intake, *arm, *LTop, *LBot;

	int outputCounter, RPMoffset, ShooterState, LTopSpeed, LBotSpeed;

	bool FlagB5, FlagB6;

    public:
	DoctaEight::DoctaEight(void)
	{
	    /*
         * pv process var -error-> sp set point
         * mv manip var
         * proportional < change
         * integral < change rate of proportional controll
         * derivative < temper controll
         *
         * ramp is high proportional, or I and P(MV) < pv found by encoder

         ____.Faults(____::kCurrentFault);
         * kTemperatureFault = 2, kBusVoltageFault = 4, kGateDriverFault = 8
         *can use above to set fault for some things
         * void CANJaguar::ConfigNeutralMode(NeutralMode)
         * fault time can also be set
        */

        /// DS Start and Kill Watchdog
		REDRUM;
		driverOut = DriverStationLCD::GetInstance();

        ///POINTER INITIALIZE
		pilot = new Joystick (1);
		copilot = new Joystick (2);

        lefty = new CANJaguar (4,CANJaguar::kPercentVbus);
        lefty->ConfigMaxOutputVoltage(kVOLTAGE);
        lefty->SetVoltageRampRate(RAMP);
        lefty->EnableControl();

        leftyB = new CANJaguar (5,CANJaguar::kPercentVbus);
        leftyB->ConfigMaxOutputVoltage(kVOLTAGE);
        leftyB->SetVoltageRampRate(RAMP);
        leftyB->EnableControl();

        righty = new CANJaguar (2,CANJaguar::kPercentVbus);
        righty->ConfigMaxOutputVoltage(kVOLTAGE);
        righty->SetVoltageRampRate(RAMP);
        righty->EnableControl();

        rightyB = new CANJaguar (3,CANJaguar::kPercentVbus);
        rightyB->ConfigMaxOutputVoltage(kVOLTAGE);
        rightyB->SetVoltageRampRate(RAMP);
        rightyB->EnableControl();

        arm = new CANJaguar (9,CANJaguar::kPercentVbus);
        arm->ConfigMaxOutputVoltage(kVOLTAGE);
        arm->SetVoltageRampRate(RAMP);
        arm->EnableControl();

        intake = new CANJaguar (8,CANJaguar::kPercentVbus);
        intake->ConfigMaxOutputVoltage(kVOLTAGE);
        intake->SetVoltageRampRate(RAMP);
        intake->EnableControl();

        LTop = new CANJaguar (7,CANJaguar::kSpeed);
        LTop->SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
        LTop->SetPositionReference(CANJaguar::kSpeedRef_QuadEncoder);
        LTop->ConfigEncoderCodesPerRev(kENCCOUNT);
        LTop->EnableControl();
        LTop->SetVoltageRampRate(RAMP);
        LTop->SetPID(KP,KI,KD);
        LTop->ConfigMaxOutputVoltage(kVOLTAGE);

        LBot = new CANJaguar (6,CANJaguar::kSpeed);
        LBot->SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
        LBot->SetPositionReference(CANJaguar::kSpeedRef_QuadEncoder);
        LBot->ConfigEncoderCodesPerRev(kENCCOUNT);
        LBot->EnableControl();
        LBot->SetVoltageRampRate(RAMP);
        LBot->SetPID(KP,KI,KD);
		LBot->ConfigMaxOutputVoltage(kVOLTAGE);

		//MAXIMUM
		//V CHANGE

										//OR I + P
		///INTERGER INITIALIZE
		outputCounter = 0;
		RPMoffset = 0;
		ShooterState = 0;
		LTopSpeed = 0;
		LBotSpeed = 0;

        ///BOOLEAN INITIALIZE
		FlagB5 = false;
		FlagB6 = false;
    }

	void DoctaEight::Autonomous(void)
	{
		REDRUM;
		output();
	}

	void DoctaEight::OperatorControl(void)
	{
		REDRUM;
		LTop->SetSafetyEnabled(0);
		LTop->EnableControl();
		LBot->SetSafetyEnabled(0);
		LBot->EnableControl();
		while (IsOperatorControl())
		{
			REDRUM;
			output();

			//Set Shooter state and reset RPMoffset if necessary
			if (copilot->GetRawButton(1)) //BUTTON A
			{
				REDRUM;
				if(ShooterState != 1)
				{
					REDRUM;
					ShooterState = 1;
					RPMoffset = 0;
				}
			}
			else if (copilot->GetRawButton(2)) //BUTTON B
			{
				REDRUM;
				if(ShooterState != 2)
				{
					REDRUM;
					ShooterState = 2;
					RPMoffset = 0;
				}
			}
			else if (copilot->GetRawButton(3)) //BUTTON X
			{
				REDRUM;
				if(ShooterState != 3)
				{
					REDRUM;
					ShooterState = 3;
					RPMoffset = 0;
				}
			}
			else if (copilot->GetRawButton(4)) //BUTTON Y
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
			if(copilot->GetRawButton(5)) //BUTTON LB
			{
				REDRUM;
				FlagB5 = true;
			}
			else if(copilot->GetRawButton(6)) //BUTTON RB
			{
				REDRUM;
				FlagB6 = true;
			}

			if(FlagB5 == true && copilot->GetRawButton(5) == false)
			{
				REDRUM;
				RPMoffset -= 50;
				FlagB5 = false;
			}
			else if(FlagB6 && !copilot->GetRawButton(6))
			{
				REDRUM;
				RPMoffset += 50;
				FlagB6 = false;
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
			LTop->Set(LTopSpeed);
			LBot->Set(LBotSpeed);


			//move simple platform arm
			leftyrighty(-pilot->GetY(), -pilot->GetRawAxis(4));
			intake->Set(-copilot->GetY());
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
		righty->Set(right*.9);
		rightyB->Set(right*.9);
		lefty->Set(left*.9);
		leftyB->Set(left*.9);
	}

};
START_ROBOT_CLASS(DoctaEight); //DOCTOR OCTAGONAPUS WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
