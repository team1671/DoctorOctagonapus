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
	int negate=(m_bReversed)?(-1):(1);
	m_canLeftFront->Set(_fLeftFront*negate);
	m_canRightFront->Set(_fRightFront*negate);
	m_canLeftBack->Set(_fLeftBack*negate);
	m_canRightBack->Set(_fRightBack*negate);
};

void CANDrive::SetReversed(bool _bReverse)
{
	m_bReversed = _bReverse;
};

//TANK DRIVE
void CANDrive::TankDrive(float _fLeftSpeed, float _fRightSpeed)
{
	ChangeControlMode(CANJaguar::kPercentVbus);
	SetValues(_fLeftSpeed, _fRightSpeed, _fLeftSpeed, _fRightSpeed);
};

void CANDrive::SetPID(float _Kp, float _Ki, float _Kd)
{
	m_canLeftFront->SetPID(_Kp, _Ki, _Kd);
	m_canRightFront->SetPID(_Kp, _Ki, _Kd);
	m_canLeftBack->SetPID(_Kp, _Ki, _Kd);
	m_canRightBack->SetPID(_Kp, _Ki, _Kd);
};

bool CANDrive::DistDrive(float _fLeftDist, float _fRightDist)
{
	float inchestopulses=1/*inch*//(3.14159/*pi*/*4.125/*wheel diameter*/);
	if(GetControlMode()!=CANJaguar::kPosition)
	{
		ChangeControlMode(CANJaguar::kPosition);
		m_canLeftFront->EnableControl();
		m_canRightFront->EnableControl();
		m_canLeftBack->EnableControl();
		m_canRightBack->EnableControl();
	}
	float LRots=_fLeftDist*inchestopulses;//DONT REMOVE ABBREVATIONS!
	float RRots=_fRightDist*inchestopulses;
	m_canLeftFront->Set(LRots);
	m_canRightFront->Set(RRots);
	m_canLeftBack->Set(LRots);
	m_canRightBack->Set(Rots);
	m_canLeftFront->GetPosition();
	//if(()||()||()||())
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

void CANDrive::Test (void)
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

void CANDrive::ChangeControlMode(CANJaguar::ControlMode controlMode)
{//change if duplicate required, also change getcontrolmode
	m_canLeftFront->ChangeControlMode(controlMode);
	m_canRightFront->ChangeControlMode(controlMode);
	m_canLeftBack->ChangeControlMode(controlMode);
	m_canRightBack->ChangeControlMode(controlMode);
}

CANJaguar::ControlMode CANDrive::GetControlMode()
{
	return m_canLeftFront->GetControlMode();
}
