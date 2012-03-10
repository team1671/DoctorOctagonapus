#include "Shooter.h"

Shooter::Shooter(void)
{
	m_canTopShooter = NULL;
	m_canBottomShooter = NULL;
};

Shooter::Shooter(int _nTopShot, int _nBotShot, int _nEncoderCount)
{
	m_canTopShooter = new CANJaguar(_nTopShot);
	m_canTopShooter->ChangeControlMode(CANJaguar::kSpeed);
	m_canTopShooter->SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	m_canTopShooter->ConfigEncoderCodesPerRev(_nEncoderCount);
	m_canTopShooter->EnableControl();
	
	m_canBottomShooter = new CANJaguar(_nBotShot);
	m_canBottomShooter->ChangeControlMode(CANJaguar::kSpeed);
	m_canBottomShooter->SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	m_canBottomShooter->ConfigEncoderCodesPerRev(_nEncoderCount);
	m_canBottomShooter->EnableControl();
	
	m_fTopShotValue = 0.0;
	m_fBotShotValue = 0.0;
	
	m_flagIncrement = false;
	m_flagDecrement = false;
};

void Shooter::UpdateValues(void)
{
	m_canTopShooter->Set(m_fTopShotValue);
	m_canBottomShooter->Set(m_fBotShotValue);
};

void Shooter::Control(Joystick *_stick)
{
	if(_stick->GetRawButton(6)) m_flagIncrement = true;
	if(_stick->GetRawButton(5)) m_flagDecrement = true;
	
	if(!_stick->GetRawButton(6) && m_flagIncrement)
	{
		m_fBotShotValue -= 10; //more negative FASTER
		m_flagIncrement = false;
	}
	if(!_stick->GetRawButton(5) && m_flagDecrement)
	{
		m_fBotShotValue += 10; //less negative SLOWER
		m_flagDecrement = false;
	}
}

void Shooter::SetPID(float _Kp, float _Ki, float _Kd)
{
	m_canTopShooter->SetPID(_Kp, _Ki, _Kd);
	m_canBottomShooter->SetPID(_Kp, _Ki, _Kd);
}

void Shooter::ChangeSpeed(float _fTopShotValue, float _fBotShotValue)
{
	m_fTopShotValue = _fTopShotValue;
	m_fBotShotValue = _fBotShotValue;
};

void Shooter::SetMode(CANJaguar::ControlMode _cmTopShotMode, CANJaguar::ControlMode _cmBotShotMode)
{
	m_canTopShooter->ChangeControlMode(_cmTopShotMode);
	m_canBottomShooter->ChangeControlMode(_cmBotShotMode);
};

float Shooter::GetTopSpeed(void)
{
	return m_canTopShooter->GetSpeed();
}

float Shooter::GetBottomSpeed(void)
{
	return m_canBottomShooter->GetSpeed();
}

float Shooter::GetTopInput(void)
{
	return m_fTopShotValue;
}

float Shooter::GetBottomInput(void)
{
	return m_fBotShotValue;
}

bool Flagger(bool test, bool &old, bool alternate)//true=signal with change false=signal when on once
{
	if(test==true&&old==false)
		return true;
	else if(test==false&&old==true&&alternate)
		return true;
	else
		return false;
}
