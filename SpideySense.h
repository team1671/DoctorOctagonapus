void DoctaEight::targetSelect(void)
{
	GetWatchdog().Kill();
	camera.GetImage(&image);
	//gets image from cam
	vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
	//finds targets
	
	itt++;
	if (itt%2500 == 0)
	{
		itt = 0;
		cout << "Number of targets: " << particles->size() << endl;
	}

	
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
	cout << "AIM" << endl;
	//find what point motors stop then this should be slightly above
	while (decrement > .3 or decrement < -.3 and copilot.GetRawButton(1) and choiceTarget != 7)
	{

		targetSelect();
		//select target to shoot at-- will potentially change in while because turning may reveal better
		//targets
		
		camera.GetImage(&image);
		//gets image from cam
		vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
		//finds targets
		ParticleAnalysisReport& par = (*particles)[choiceTarget];
		//get report on target

		//RainbowDash();
		//output dashboard values
		
		itt++;
		if (itt%2500 == 0)
		{
			itt = 0;
			cout << "Number of targets: " << particles->size() << endl << "Target selected: " << choiceTarget
				<< endl << "Zeroing in: " << par.center_mass_x_normalized;
		}
		//output number of targets
		
		
		if (par.center_mass_x_normalized > -.3  && par.center_mass_x_normalized < .3)
		{
			decrement = par.center_mass_x_normalized*2;
		}
		else
			decrement = 1;
		//lower speed if aiming at target or set back to 1
		
		
		//decrement is for keeping drive from moving back and forth continuously
		if(par.center_mass_x_normalized > 0)//turn right; 
		{
			righty.Set(0);
			rightyB.Set(0);
			lefty.Set(decrement);
			leftyB.Set(decrement);
		}
		else if(par.center_mass_x_normalized < 0)//turn left
		{
			lefty.Set(0);
			leftyB.Set(0);
			righty.Set(decrement);
			rightyB.Set(decrement);
		}
		
		Wait (.005f);//wait a sec- the longer, the more will travel before decriment is reset
	}
	if (choiceTarget == 7){cout << "no target";}
}
