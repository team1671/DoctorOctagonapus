#ifndef _CUSTOMDRIVE_H_
#define _CUSTOMDRIVE_H_

#define MAX_MOTORS 8

#include "WPILib.h"
#include "math.h"

class Jaguar;
class Joystick;
class Encoder;
//class Gyro;

class CustomDrive
{
private:
	int cd_nJags;
	Jaguar *cd_pMotors[MAX_MOTORS];
	PIDController *cd_pPID[MAX_MOTORS];
	float mot_dir[MAX_MOTORS];
	Encoder *cd_pEncoders[MAX_MOTORS];
//	Gyro *gyro;

public:
	CustomDrive(void);
	CustomDrive(int _nJags);
	CustomDrive(int _nJags, int _gyroPort);

	void TankDrive(float _lSpeed, float _rSpeed);
	void TankDrive(Joystick* _lStick, Joystick* _rStick);
};


#endif
