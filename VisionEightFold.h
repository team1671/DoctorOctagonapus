
double DoctaEight::fOfX(double x)
{
	return 	(atan((107.5-CAMERAHEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
			+atan((CAMERAHEIGHT-31.5)/(x))); //angle from bottom triangle
}

double DoctaEight::getDistance()
{
	camera.GetImage(&image);
	vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
	
	double aproximation=0;
	
	if (limitedDistance == 1)
	{
		ParticleAnalysisReport& par = (*particles)[distanceTarget];
		
		aproximation = 9//half height of target in inches over target to get adjacent
						/tan(//tan of this to get opposite over adjacent
								54*//angle of lens vision
									((par.particleArea/24)//to get height in pixels
										/par.imageHeight)//above divided to get ratio of size
											/2);//to get half of angle and therefore right triangle
	}
	else
	{
		ParticleAnalysisReport& bottom = (*particles)[distanceTarget];//bottom target
		ParticleAnalysisReport& top = (*particles)[choiceTarget];//top target 
		
		double theta=(top.center_mass_y_normalized-bottom.center_mass_y_normalized)/240*54;//the distance is turned into an angle (refer to fofx(x))
		//107.5 is top target height 31.5 is bottom target height
		
		double accuracy=1;
		double dotbinary=54;
		while((accuracy<1)||(accuracy>-1))//binary approximation-> guesses using 1/2 distances until tlar -- function too complex
		{
			dotbinary/=2; //this is the number which modifies the approximation
			if(fOfX(aproximation+dotbinary)>theta) //if the value to be added overshoots it does not add
				aproximation+=dotbinary;
			
			accuracy=theta-fOfX(aproximation);
		}
	}
	return aproximation;
}


void DoctaEight::shoot(void)
{
	
	getDistance();
	//if 0, too close to see target-- set jags low
	
		
	//SHOOT HERE!
}
