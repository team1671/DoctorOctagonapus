#ifndef CAN_DRIVE_H
#define CAN_DRIVE_H

#include "WPILib.h"
#include "KinectStick.h"

class CANDrive
{
private:
	CANJaguar 	*m_canLeftFront,
				*m_canRightFront,
				*m_canLeftBack,
				*m_canRightBack;
	
	bool		m_bReversed;
	
	float		m_fLeftPosition, m_fRightPosition;
	
public:
	CANDrive(void);
	//CANDrive(const CANDrive &);
	CANDrive(int _nLeftFront, int _nRightFront, int _nLeftBack, int _nRightBack);

	void SetPID(float _Kp, float _Ki, float _Kd);
	
	void SetValues(float _fLeftFront, float _fRightFront, float _fLeftBack, float _fRightBack);
	void SetReversed(bool _bReverse);
	void TankDrive(float _fLeftSpeed, float _fRightSpeed);
	void DistDrive(float _fLeftDist, float _fRightDist);
	void TankDrive(Joystick *_stick);
	void TankDrive(KinectStick *_leftarm, KinectStick *_rightarm);
	
	void GetValues(float &_LF, float &_LB, float &_RF, float &_RB);
	
	void ArcadeDrive (float _fThrottleSpeed, float _fTurnSpeed);
	void ArcadeDrive (Joystick *_stick);
	
	void SpeedTest (void);
};



#endif
