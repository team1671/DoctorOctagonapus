#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "WPILib.h"

class Shooter
{
public:
//	CANJaguar	*m_canTopShooter,
//				*m_canBottomShooter;
	
	float		m_fTopShotValue,
				m_fBotShotValue;
	
	bool		m_flagIncrement,
				m_flagDecrement;
	
	
	CANJaguar	*m_canTopShooter,
				*m_canBottomShooter;
	
	Shooter(void);
	//Shooter(const Shooter &);
	Shooter(int _nTopShot, int _nBotShot, int _nEncoderCount);
	
	void Control(Joystick *_stick);
	
	void SetPID(float _Kp, float _Ki, float _Kd);
	
	void UpdateValues(void);
	

	void ChangeSpeed(float _fTopShotValue, float _fBotShotValue);
	
	void SetMode(	CANJaguar::ControlMode _cmTopShotMode, 
							CANJaguar::ControlMode _cmBotShotMode);
	
	float GetTopSpeed(void);
	float GetBottomSpeed(void);
	float GetTopInput(void);
	float GetBottomInput(void);
};

#endif
