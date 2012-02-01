void DoctaEight::targetSelect(void)
{
	camera.GetImage(&image);
	//gets image from cam
	vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
	//finds targets
	
	itt++;
	if (itt%2500 == 0)
	{
		itt = 0;
		cout << "Number of targets: " << particles->size();
	}

	
	/*
	 * below -- first, if at least three targets, use accurate distance tracking
	 * second, if only two targets visible, track for higher one
	 * third, if one target, track it
	 * 
	 * note: this is for distance tracking only; if far enough away, all targets will be visible after
	 * aim() is called
	 */
	
	if (3 <= particles->size())
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
		else {distanceTarget = 4;}
		
		limitedDistance = 0;
	
	}
	else if (2 == particles->size())
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
		limitedDistance = 1;
	}
	else if (1 == particles->size())
	{
		choiceTarget = 1;
		limitedDistance = 1;
	}
}

void DoctaEight::aim(void)
{
	choiceTarget = 7;
	//default
	targetSelect();
	//if targetSelect finds a target, below will happen
	
	//find what point motors stop then this should be slightly above
	while (decrement > .2 or decrement < -.2 && copilot.GetRawButton(1) && choiceTarget != 7)
	{

		targetSelect();
		//select target to shoot at-- will potentially change in while because turning may reveal better
		//targets

		//RainbowDash();
		//output dashboard values
		
		camera.GetImage(&image);
		//gets image from cam
		vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
		//finds targets
		ParticleAnalysisReport& par = (*particles)[choiceTarget];
		//get report on target
		cout << "Number of targets: " << particles->size() << "\nTarget selected: " << choiceTarget
				<< "/nTurning to target-(0 is directly on)" << par.center_mass_x_normalized;
		//output number of targets
		
		Wait(.005f);//motor uptade time
		
		if (par.center_mass_x_normalized > -.3  && par.center_mass_x_normalized < .3)
		{
			decrement = par.center_mass_x_normalized*2;
		}
		else
			decrement = 1;
		//lower speed if aiming at target or set back to 1
		
		
		if(par.center_mass_x_normalized > 0)//right; 
		{
			righty.Set(0);
			rightyB.Set(0);
			lefty.Set(decrement);
			leftyB.Set(decrement);
		}
		else if(par.center_mass_x_normalized < 0)//left
		{
			lefty.Set(0);
			leftyB.Set(0);
			righty.Set(decrement);
			rightyB.Set(decrement);
		}
	}
	//above aims
}
