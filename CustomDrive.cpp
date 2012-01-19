 #include "CustomDrive.h"

//When assigning motors, ODD is LEFT; EVEN is RIGHT

CustomDrive::CustomDrive(void)
{
	//By default, the array is set to all 0's
	cd_nJags = 0;
	for(register int i = 0; i < MAX_MOTORS; ++i)
	{
		cd_pMotors[i] = 0;
	};
	gyro = NULL;
};

CustomDrive::CustomDrive(int _nJags)
{
	//Assigns the number of jags to the ports.
	cd_nJags = _nJags;
	for(register int i = 0; i < _nJags; ++i)
	{
		cd_pMotors[i] = new Jaguar(i+1);
	};

};


CustomDrive::CustomDrive(int _nJags, int _gyroPort)
{
	//Assigns the number of jags to the ports.
	cd_nJags = _nJags;
	for(register int i = 0; i < _nJags; ++i)
	{
		cd_pMotors[i] = new Jaguar(i+1);
	};
	gyro = new Gyro(_gyroPort);
};

//*****TANK DRIVE*****

inline void CustomDrive::TankDrive(float _lSpeed, float _rSpeed)
{
	//Sets motor speeds the the arguments in the function
	for(register int i = 0; i < cd_nJags; i+=2)
	{
		cd_pMotors[i]->Set(_rSpeed);
		cd_pMotors[i+1]->Set(_lSpeed);
	};
};

inline void CustomDrive::TankDrive(Joystick* _lStick, Joystick* _rStick)
{
	//Calls the previous function and inserts the Joystick pointers as the arguments for the function
	TankDrive(_lStick->GetY(), _rStick->GetY());
};
