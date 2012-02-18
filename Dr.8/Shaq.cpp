#include"Vrisk.h"

void DoctaEight::aim()
{
	Kill;
	//robot width-33			//inches
	//robot length-18			//inches
	
	//say stuff
	
	aimin = 1;
	while (copilot.GetRawButton(1) or IsAutonomous())
	{
		Kill;
		output();
		double decrement = 1;
		double n;
		if (choiceTarget!=-1 and CamData.numTargets < 5)//if there is a target
		{
			while (IsAutonomous() or copilot.GetRawButton(1))
			{
				UpdateCamData();
				output();

				if (CamData.centerX > -.2 or CamData.centerX < .2)
				{
					decrement = n*CamData.centerX*CamData.centerX*CamData.centerX;
				}
				if (CamData.centerX < 0)//right
					leftyrighty(decrement, -decrement);
				else if (CamData.centerX > 0)//left
					leftyrighty(-decrement, decrement);
				if (CamData.centerX > -.5 or CamData.centerX < .5)
				{
					leftyrighty(0, 0);
					return;
				}
				if (lefty.GetSpeed() < 0 or righty.GetSpeed() < 0)
					n++;
			}
		}
	}
	aimin = 0;
}

void DoctaEight::shoot(void)
{
	//say stuff
	Kill;
	shootin =1;
	double approximation = 0;
	while (approximation != -1 && copilot.GetRawButton(2))//while in aim() should count also
	{
		output();
		
		/*
		 * CamValues.approx
		 * CAMERAHEIGHT
		 * kANGLEOFLAUNCH
		 * 
		 BOTTOM JAG MUST BE NEGATIVE
		 
		 * double X = [(V^2)/g * (Squrt(3)/2)]+23.09in
		 * 
		 * setjags(returns of F(X))//F() exp function
		 * 
		 * while (encoders show motors below speed)
		 * 		speed motors
		 *		output();
		 * 
		 * remember not need select target as will in get dist
		 */
	}
	//SHOOT HERE!
	shootin = 0;
}
