<<<<<<< HEAD
#include "CANDrive.h"
#include "KinectStick.h"

CANDrive::CANDrive(void)
{
	m_canLeftBack = NULL;
	m_canRightBack = NULL;
	m_canLeftFront = NULL;
	m_canRightFront = NULL;
	
	m_bReversed = false;
	
	//m_fLeftPosition = 0;
	//m_fRightPosition = 0;
	
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
	if(GetControlMode()!=CANJaguar::kPercentVbus)
	{
		ChangeControlMode(CANJaguar::kPercentVbus);
		m_canLeftFront->EnableControl();
		m_canRightFront->EnableControl();
		m_canLeftBack->EnableControl();
		m_canRightBack->EnableControl();
	}
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
	float inchestorots=1/*inch*//(3.14159/*pi*/*4.125/*wheel diameter*/);
	if(CANJaguar::kPosition != m_canLeftFront->GetControlMode())
	{
		ChangeControlMode(CANJaguar::kPosition);
		m_canLeftFront->ConfigEncoderCodesPerRev(250);
		m_canRightFront->ConfigEncoderCodesPerRev(250);
		m_canLeftBack->ConfigEncoderCodesPerRev(250);
		m_canRightBack->ConfigEncoderCodesPerRev(250);
		m_canLeftFront->SetPID(1.0,0,0);
		m_canRightFront->SetPID(1.0,0,0);
		m_canLeftBack->SetPID(1.0,0,0);
		m_canRightBack->SetPID(1.0,0,0);
		m_canLeftFront->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canRightFront->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canLeftBack->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canRightBack->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canLeftFront->EnableControl();
		m_canRightFront->EnableControl();
		m_canLeftBack->EnableControl();
		m_canRightBack->EnableControl();
	}
	float LRots=_fLeftDist*inchestorots;//DONT REMOVE ABBREVATIONS!
	float RRots=_fRightDist*inchestorots;
	m_canLeftFront->Set(LRots);
	m_canRightFront->Set(RRots);
	m_canLeftBack->Set(LRots);
	m_canRightBack->Set(RRots);
	float LRef=m_canLeftFront->GetPosition();
	float RRef=m_canLeftFront->GetPosition();
	if(((LRef-LRots>0)?(LRef-LRots):(LRots-LRef))<0.5||((RRef-RRots>0)?(RRef-RRots):(RRots-RRef))<0.5)
		return true;
	return false;
}

bool CANDrive::ThetaDrive(float theta)
{
	float angletoinches=theta/360*/*degrees in a circle*/37.831*3.14159/*circle circumfrence*//0.87228/*inches actually moved around circle*/;
	return DistDrive(angletoinches,-angletoinches);
}

void CANDrive::TankDrive(Joystick *_stick)
{
	TankDrive(-_stick->GetTwist(), -_stick->GetY());
};

void CANDrive::AutoTankDrive(float dir)
{
	if(abs(dir) < .1)// fix this later
		TankDrive(dir*8, -dir*8);
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

//ArcadeDriveKinect
void CANDrive::ArcadeDriveKinect(float _fThrottleSpeed)
{
	float _fLeftSpeed, _fRightSpeed;
	_fLeftSpeed = _fThrottleSpeed;
	_fRightSpeed = _fThrottleSpeed;

	TankDrive(_fLeftSpeed, _fRightSpeed);
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
=======
#include "CANDrive.h"
#include "KinectStick.h"

CANDrive::CANDrive(void)
{
	m_canLeftBack = NULL;
	m_canRightBack = NULL;
	m_canLeftFront = NULL;
	m_canRightFront = NULL;
	
	m_bReversed = false;
	
	//m_fLeftPosition = 0;
	//m_fRightPosition = 0;
	
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
	if(GetControlMode()!=CANJaguar::kPercentVbus)
	{
		ChangeControlMode(CANJaguar::kPercentVbus);
		m_canLeftFront->EnableControl();
		m_canRightFront->EnableControl();
		m_canLeftBack->EnableControl();
		m_canRightBack->EnableControl();
	}
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
	float inchestorots=1/*inch*//(3.14159/*pi*/*4.125/*wheel diameter*/);
	if(CANJaguar::kPosition != m_canLeftFront->GetControlMode())
	{
		ChangeControlMode(CANJaguar::kPosition);
		m_canLeftFront->ConfigEncoderCodesPerRev(250);
		m_canRightFront->ConfigEncoderCodesPerRev(250);
		m_canLeftBack->ConfigEncoderCodesPerRev(250);
		m_canRightBack->ConfigEncoderCodesPerRev(250);
		m_canLeftFront->SetPID(1.0,0,0);
		m_canRightFront->SetPID(1.0,0,0);
		m_canLeftBack->SetPID(1.0,0,0);
		m_canRightBack->SetPID(1.0,0,0);
		m_canLeftFront->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canRightFront->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canLeftBack->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canRightBack->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		m_canLeftFront->EnableControl();
		m_canRightFront->EnableControl();
		m_canLeftBack->EnableControl();
		m_canRightBack->EnableControl();
	}
	float LRots=_fLeftDist*inchestorots;//DONT REMOVE ABBREVATIONS!
	float RRots=_fRightDist*inchestorots;
	m_canLeftFront->Set(LRots);
	m_canRightFront->Set(RRots);
	m_canLeftBack->Set(LRots);
	m_canRightBack->Set(RRots);
	float LRef=m_canLeftFront->GetPosition();
	float RRef=m_canLeftFront->GetPosition();
	if(((LRef-LRots>0)?(LRef-LRots):(LRots-LRef))<0.5||((RRef-RRots>0)?(RRef-RRots):(RRots-RRef))<0.5)
		return true;
	return false;
}

bool CANDrive::ThetaDrive(float theta)
{
	float angletoinches=theta/360*/*degrees in a circle*/37.831*3.14159/*circle circumfrence*//0.87228/*inches actually moved around circle*/;
	return DistDrive(angletoinches,-angletoinches);
}

void CANDrive::TankDrive(Joystick *_stick)
{
	TankDrive(-_stick->GetTwist(), -_stick->GetY());
};

void CANDrive::AutoTankDrive(float dir)
{
	if(abs(dir) < .1)// fix this later
		TankDrive(dir*8, -dir*8);
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

//ArcadeDriveKinect
void CANDrive::ArcadeDriveKinect(float _fThrottleSpeed)
{
	float _fLeftSpeed, _fRightSpeed;
	_fLeftSpeed = _fThrottleSpeed;
	_fRightSpeed = _fThrottleSpeed;

	TankDrive(_fLeftSpeed, _fRightSpeed);
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
>>>>>>> b575175822067cc77622453ead236f74494a7ac7
