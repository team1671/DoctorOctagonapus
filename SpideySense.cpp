



//	see	doc8.h	for	todo





#include "DoctorEight.h"

void DoctaEight::targetSelect(void)
{
	GetWatchdog().Kill();
	camera.GetImage(&image);
	//gets image from cam
	vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
	//finds targets

	driverOut->Clear();	
	driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "Target Select, numTargets: %n", (int)particles->size());
	driverOut->UpdateLCD();

	
	/*
	 * below -- first, if at least three targets, use accurate distance tracking and track higher target
	 * second, if only two targets visible, track for higher one
	 * third, if one target, track it
	 * 
	 * note: this is for distance tracking only; if far enough away, all targets will be visible after aim() is called
	 */
	
	if (3 <= particles->size())//if 3 or 4 targets visible
	{
		ParticleAnalysisReport& par = (*particles)[1];
		firstTarget = par.center_mass_y;
		par = (*particles)[2];
		secondTarget = par.center_mass_y;
		par = (*particles)[3];
		thirdTarget = par.center_mass_y;
		
		if (firstTarget > secondTarget && firstTarget > thirdTarget)
		{
			choiceTarget = 1;
		}
		else if (secondTarget > thirdTarget && secondTarget > firstTarget)
		{
			choiceTarget = 2;
		}
		else if (thirdTarget > secondTarget && thirdTarget > firstTarget)
		{
			choiceTarget = 3;
		}
		else {choiceTarget = 4;}
		//above chooses target to shoot at - highest target
		
		if (firstTarget < secondTarget && firstTarget < thirdTarget)
		{
			distanceTarget = 1;
		}
		else if (secondTarget < thirdTarget && secondTarget < firstTarget)
		{
			distanceTarget = 2;
		}
		else if (thirdTarget < secondTarget && thirdTarget < firstTarget)
		{
			distanceTarget = 3;
		}
		else 
		{
			distanceTarget = 4;
		}

		limitedDistance = 0;//because this is three targets, use accurate distance algorithm
	
	}
	else if (2 == particles->size())//if 2 targets visible
	{
		ParticleAnalysisReport& par = (*particles)[1];
		firstTarget = par.center_mass_y;
		par = (*particles)[2];
		secondTarget = par.center_mass_y;
		
		if (firstTarget > secondTarget)
		{
			choiceTarget = 1;
		}
		else
		{
			choiceTarget = 2;
		}
		
		limitedDistance = 1;//use single target target tracking
	
	}
	else if (1 == particles->size())
	{
		choiceTarget = 1;
		limitedDistance = 1;
	}
	else{choiceTarget = 7;}
	
}

void DoctaEight::aim(void)
{
	GetWatchdog().Kill();
	targetSelect();
	//find what point motors stop then this should be slightly above
	while (copilot.GetRawButton(1) and choiceTarget != 7 or IsAutonomous() and choiceTarget != 7)
	{//PID to also stop loop in second half
		GetWatchdog().Kill();

		
		targetSelect();
		//select target to shoot at-- will potentially change in while because turning may reveal better
		//targets
		
		camera.GetImage(&image);
		//gets image from cam
		vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
		//finds targets
		ParticleAnalysisReport& par = (*particles)[choiceTarget];
		//get report on target
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "Aiming");
		driverOut->PrintfLine(DriverStationLCD::kUser_Line2, "Number of targets: %d", particles->size());
		driverOut->PrintfLine(DriverStationLCD::kUser_Line3, "Target selected: %n", choiceTarget);
		driverOut->PrintfLine(DriverStationLCD::kUser_Line4, "Zeroing in: %d", par.center_mass_x_normalized);
		driverOut->UpdateLCD();
		//output number of targets
		
		
		//decrement is for keeping drive from moving back and forth continuously
		//turn
		{
			driverOut->PrintfLine(DriverStationLCD::kUser_Line5, 
			(
				(par.center_mass_x_normalized > 0)?("TURNING RIGHT"):("TURNING LEFT"))//NOSEMICOLON!
			);
			driverOut->UpdateLCD();
			double offset=par.center_mass_x_normalized/54;
			double safety=0.1;
			leftyrighty(1*offset*safety,-1*offset*safety);
			//righty.Set(1*offset);
			//rightyB.Set(1*offset);
			//lefty.Set(-1*offset);//THIS MUST BE FIXED WITH PID
			//leftyB.Set(-1*offset);
		}/*
		else if(par.center_mass_x_normalized < 0)//turn left
		{
			driverOut->PrintfLine(DriverStationLCD::kUser_Line5, "TURNING LEFT");
			driverOut->UpdateLCD();
			lefty.Set(0);
			leftyB.Set(0);
			righty.Set(1);//THIS MUST BE FIXED WITH PID
			rightyB.Set(1);
		}*/
	}
	if (choiceTarget == 7)
	{
		driverOut->Clear();
		driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "no target");
		driverOut->UpdateLCD();
	}
}
