#include "CANDrive.h"
#include "KinectStick.h"

CANDrive::CANDrive(void)
{
	m_canLeftBack = NULL;
	m_canRightBack = NULL;
	m_canLeftFront = NULL;
	m_canRightFront = NULL;
	
	m_bReversed = false;
	
	m_fLeftPosition = 0;
	m_fRightPosition = 0;
	
};

CANDrive::CANDrive(int _nLeftFront, int _nRightFront, int _nLeftBack, int _nRightBack)
{
	m_canLeftFront = new CANJaguar(_nLeftFront);
	m_canRightFront = new CANJaguar(_nRightFront);
	m_canLeftBack = new CANJaguar(_nLeftBack);
	m_canRightBack = new CANJaguar(_nRightBack);
	
	m_bReversed = false;
	
};

void CANDrive::GetValues(float &_LF, float &_LB, float &_RF, float &_RB)
{
	_LF = m_canLeftFront->Get();
	_LB = m_canLeftBack->Get();
	_RB = m_canRightBack->Get();
	_RF = m_canRightFront->Get();
}

//SETTING VALUES
void CANDrive::SetValues(float _fLeftFront, float _fRightFront, float _fLeftBack, float _fRightBack)
{
	m_canLeftFront->Set(_fLeftFront);
	m_canRightFront->Set(_fRightFront);
	m_canLeftBack->Set(_fLeftBack);
	m_canRightBack->Set(_fRightBack);
	
};

void CANDrive::SetReversed(bool _bReverse)
{
	m_bReversed = _bReverse;
};

//TANK DRIVE
void CANDrive::TankDrive(float _fLeftSpeed, float _fRightSpeed)
{
	m_canRightBack->ChangeControlMode(CANJaguar::kPercentVbus);
	m_canLeftBack->ChangeControlMode(CANJaguar::kPercentVbus);
	m_canRightFront->ChangeControlMode(CANJaguar::kPercentVbus);
	m_canLeftFront->ChangeControlMode(CANJaguar::kPercentVbus);
	
	if(m_bReversed)
	{
		SetValues(_fLeftSpeed, _fRightSpeed, _fLeftSpeed, _fRightSpeed);
	} else
	{
		SetValues(-_fRightSpeed, -_fLeftSpeed, -_fRightSpeed, -_fLeftSpeed);
	}
};

void CANDrive::SetPID(float _Kp, float _Ki, float _Kd)
{
	m_canLeftFront->SetPID(_Kp, _Ki, _Kd);
	m_canRightFront->SetPID(_Kp, _Ki, _Kd);
	m_canLeftBack->SetPID(_Kp, _Ki, _Kd);
	m_canRightBack->SetPID(_Kp, _Ki, _Kd);
};

void CANDrive::DistDrive(float _fLeftDist, float _fRightDist)
{
//	m_canRightBack->ChangeControlMode(CANJaguar::kPosition);
//	m_canLeftBack->ChangeControlMode(CANJaguar::kPosition);
//	m_canRightFront->ChangeControlMode(CANJaguar::kPosition);
//	m_canLeftFront->ChangeControlMode(CANJaguar::kPosition);
//		
//	m_canLeftFront->Set(m_fLeftPosition + _fLeftDist);
//	m_canRightFront->Set(m_fRightPosition + _fRightDist);
//	m_canLeftBack->Set(m_fLeftPosition + _fLeftDist);
//	m_canRightBack->Set(m_fRightPosition + _fRightDist);
};
void CANDrive::TankDrive(Joystick *_stick)
{
	TankDrive(_stick->GetY(), _stick->GetTwist());
};

void CANDrive::TankDrive (KinectStick *_leftarm, KinectStick *_rightarm)
{
	TankDrive (_leftarm->GetY(), _rightarm->GetY());
};


//ARCADE DRIVE
void CANDrive::ArcadeDrive(float _fThrottleSpeed, float _fTurnSpeed)
{
	float _fLeftSpeed, _fRightSpeed;
	_fLeftSpeed = _fThrottleSpeed + _fTurnSpeed;
	_fRightSpeed = _fThrottleSpeed - _fTurnSpeed;
	
	TankDrive(_fLeftSpeed, _fRightSpeed);
};

void CANDrive::ArcadeDrive (Joystick *_stick)
{
	ArcadeDrive(_stick->GetY(), _stick->GetZ());
};

void CANDrive::SpeedTest (void)
{
	m_canRightBack->ChangeControlMode(CANJaguar::kSpeed);
//	m_canLeftBack->ChangeControlMode(CANJaguar::kSpeed);
	m_canRightFront->ChangeControlMode(CANJaguar::kSpeed);
//	m_canLeftFront->ChangeControlMode(CANJaguar::kSpeed);
	m_canRightBack->SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	m_canRightFront->SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);	
	m_canRightBack->ConfigEncoderCodesPerRev(250);
	m_canRightBack->EnableControl(0);
//	m_canLeftBack->ConfigEncoderCodesPerRev(250);
	m_canRightFront->ConfigEncoderCodesPerRev(250);
	m_canRightBack->EnableControl();
//	m_canLeftFront->ConfigEncoderCodesPerRev(250);
#define KP	1.00
#define KI	0.00
#define KD	0.00
	
	m_canRightBack->SetPID(KP, KI, KD);
	m_canRightBack->Set(1000);
}














