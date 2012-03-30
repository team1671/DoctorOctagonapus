#ifndef CAN_DRIVE_H
#define CAN_DRIVE_H

#include "WPILib.h"
#include "KinectStick.h"
#include "Cmath"

class CANDrive
{
private:
	CANJaguar 	*m_canLeftFront,
				*m_canRightFront,
				*m_canLeftBack,
				*m_canRightBack;
	bool		m_bReversed;
	
public:
	CANDrive(void);
	CANDrive(int _nLeftFront, int _nRightFront, int _nLeftBack, int _nRightBack);
	//various functions
	void SetPID(float _Kp, float _Ki, float _Kd);
	void SetReversed(bool _bReverse);
	void GetValues(float &_LF, float &_LB, float &_RF, float &_RB);
	void ChangeControlMode(CANJaguar::ControlMode controlMode);
	CANJaguar::ControlMode GetControlMode();
	//valuecontrol
	void SetValues(float _fLeftFront, float _fRightFront, float _fLeftBack, float _fRightBack);
	void TankDrive(float _fLeftSpeed, float _fRightSpeed);
	void ArcadeDrive (float _fThrottleSpeed, float _fTurnSpeed);
	bool DistDrive(float _fLeftDist, float _fRightDist);
	bool ThetaDrive(float angle);
	//Usercontrol
	void ArcadeDriveKinect (float left);
	void ArcadeDrive (Joystick *_stick);
	void AutoTankDrive(float dir);
	void TankDrive(Joystick *_stick);
	void TankDrive(KinectStick *_leftarm, KinectStick *_rightarm);

	void Test (void);
};

#endif
