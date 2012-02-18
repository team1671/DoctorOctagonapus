#include"head.h"

void DoctaEight::tardis(void)
{
	Kill;
	
	if (!rotes && copilot.GetRawButton(9) or copilot.GetRawButton(10) < -.1 && !rotes)
	{
		rotes=1;
		if(copilot.GetRawButton(9))
			bigArse+= 100;
		else if (!copilot.GetRawButton(9) && !copilot.GetRawButton(9))
			bigArse-= 100;
	}
	else
		rotes = 0;
	
	{
		if (copilot.GetRawButton(1))
		{
			LBot.Set(0);
			LTop.Set(0);
		}
		else if (copilot.GetRawButton(2))
		{
			LBot.Set(-1400+bigArse);
			LTop.Set(450+bigArse);
		}
		else if (copilot.GetRawButton(3))
		{
			LBot.Set(-1800+bigArse);
			LTop.Set(1600+bigArse);
		}
		else if (copilot.GetRawButton(4))
		{
			LBot.Set(-1800+bigArse);
			LTop.Set(1000+bigArse);
		}
		
//		double ptop = (dsio.GetAnalogInRatio(1) - 0.5) * MAX;
//		double pbot = (dsio.GetAnalogInRatio(2) - 0.5) * MAX;
		
//		LTop.Set(ptop);
//		LBot.Set(pbot);
		
	}

	
	arm.Set(copilot.GetTwist());
	//move simple platform arm
	
	intake.Set(-copilot.GetY());
	
	if (pilot.GetRawButton(1) && cycle == 0)
	{
		negate *= -1;
		cycle = 1;
	}
	else if (pilot.GetRawButton(4) && cycle == 0)
	{
		drive = (drive + 2)%4;//	Change to plus 1 for kinect drive be included
		cycle = 1;
	}
	else if (!pilot.GetRawButton(1) && !pilot.GetRawButton(4))
		cycle = 0;
	//to reverse drive			
	
	if (drive == 1)
	{
		if (pilot.GetY() > .01 && pilot.GetTwist() >= 0)
		{
			if (negate == 1)
				leftyrighty(negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
			else
				leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()));
		}
		else if (pilot.GetY() > .01 && pilot.GetTwist() <= 0)
		{
			if (negate ==1)
				leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()));
			else
				leftyrighty(negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
		}		
		else if (pilot.GetY() < -.01 && pilot.GetTwist() >= 0)
		{
			if (negate == 1)
				leftyrighty(negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
			else
				leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() - pilot.GetTwist()*2*pilot.GetY()));
		}
		else if (pilot.GetY() < -.01 && pilot.GetTwist() <= 0)
		{
			if (negate == 1)
				leftyrighty(negate*pilot.GetY(), negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()));
			else
				leftyrighty(negate*(pilot.GetY() + pilot.GetTwist()*2*pilot.GetY()), negate*pilot.GetY());
		}
		else
		{
			if (negate == 1)
				leftyrighty(negate*pilot.GetTwist(), negate*(-pilot.GetTwist()));
			else
				leftyrighty(negate*(-pilot.GetTwist()), negate*pilot.GetTwist());
		}
	}
	else if (drive == 2)
	{
		//leftyrighty (leftArm -> GetY(), rightArm -> GetTwist());
	}
	else if (drive == 3)
	{
		leftyrighty(-negate*pilot.GetY(), -negate*pilot.GetRawAxis(4));
	}
}

void DoctaEight::leftyrighty(double left, double right)//set drive motors on either side
{
	Kill;
	righty.Set(right);
	rightyB.Set(right);
	lefty.Set(left);
	leftyB.Set(left);
}
