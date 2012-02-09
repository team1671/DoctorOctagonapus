


//	see	doc8.h	for	todo



#include "DoctorEight.h"

double DoctaEight::fOfX(double x)
{
	GetWatchdog().Kill();
	return 	(atan((107.5-CAMERAHEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
			+atan((CAMERAHEIGHT-31.5)/(x))); //angle from bottom triangle
}

double DoctaEight::getDistance()
{
	GetWatchdog().Kill();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "distance finding");
	driverOut->UpdateLCD();
	
	camera.GetImage(&image);
	vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
	
	targetSelect();
	
	double aproximation=0;
	
	if (choiceTarget !=7)
	{
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

			double dotbinary=54;
			for(int i=0; i<30; i++)//binary approximation-> guesses using 1/2 distances until tlar -- function too complex
			{
				GetWatchdog().Kill();
				dotbinary/=2; //this is the number which modifies the approximation
				if(fOfX(aproximation+dotbinary)>theta) //if the value to be added overshoots it does not add
					aproximation+=dotbinary;
			}
		}
	}
	if (choiceTarget == 7)
	{
		aproximation= -1;
	}
	return aproximation;
}


void DoctaEight::shoot(void)
{
	GetWatchdog().Kill();
	driverOut->Clear();
	driverOut->PrintfLine(DriverStationLCD::kUser_Line1, "shootin");
	driverOut->UpdateLCD();
	
	//should have a while is _ or IsAutonomous portion to exit if timeout in autonomous
	//ALSO shoot must exit if no targets!
	//ALSO IF aproximation= -1 EXIT SHOOT
	
	getDistance();
	//if 0, too close to see target-- set jags low
	
		
	//SHOOT HERE!
}
