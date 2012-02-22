#include "CATS.h"

/*
	
	void Autonomous(void)
	{
		GetWatchdog().Kill();
		while(IsEnabled() && IsAutonomous())
		{
		}
	}

	void Doc8::AutoTurn()// 0 is -1 to 1
	{
		RunAll();
		
	}

	void Doc8::AutoShoot()
	{
		RunAll();
		
	}

	void Doc8::LCDOutput()
	{
		RunAll();
		
	}
*/

CATS::CATS(void):
thresholdRGBValues(RED_LOW,RED_HIGH,GREEN_LOW,GREEN_HIGH,BLUE_LOW,BLUE_HIGH)
{
	camera = &AxisCamera::GetInstance();
	ParticleFilterCriteria2 smartass[] =
			{
				{
					FIRST_CRITERIA,
					LOWER_HEIGHT_CRITERIA,
					HIGHER_HEIGHT_CRITERIA,
					HEIGHT_CALIBRATED,
					HEIGHT_REMOVE_OR_KEEP
				},
				{
					SECOND_CRITERIA,
					LOWER_AREA_COMPARRISON_CRITERIA,
					HIGHER_AREA_COMPARRISON_CRITERIA,
					PERCENT_CALIBRATED,
					PERCENT_REMOVE_OR_KEEP
				}
			};
	criteria = smartie;
	 
	camera->WriteBrightness(BRIGHTNESS);
	camera->WriteResolution(IMAGE_SIZE);
	camera->WriteColorLevel(COLOR_LEVEL);
	camera->WriteCompression(COMPRESSION);
	//lower easier on CRIO and harder on cam
	camera->WriteMaxFPS(MAXIMUM_FPS);
}

//////////////////////////////////////////////////////////////////////////////////

void CATS::RunAll(void)
{
	GetProcessedImage();
	GetValues();
}

//////////////////////////////////////////////////////////////////////////////////

void CATS::GetProcessedImage()
{
	ColorImage originalCameraImage(IMAQ_IMAGE_HSL);
	camera->GetImage(&originalCameraImage);
	if (camera->IsFreshImage())
	{
		BinaryImage *thresholdImage = originalCameraImage.ThresholdRGB(thresholdRGBValues);	// get just the red target pixel
		BinaryImage *filterAreaAndHeightImage = thresholdImage->ParticleFilter(criteria, FILTER_NUMBER);  // find the rectangles
		delete thresholdImage;
		FinalParticles = filterAreaAndHeightImage->ConvexHull(false);
		delete filterAreaAndHeightImage;
	}
}


//////////////////////////////////////////////////////////////////////////////////

void CATS::GetValues(void)
{
	vector<ParticleAnalysisReport> *particles = FinalParticles->GetOrderedParticleAnalysisReports();  // get the results

	ParticleAnalysisReport *topTarget;
	ParticleAnalysisReport *particle;
	ParticleAnalysisReport *bottomTarget;
	
	for (unsigned int i = 1; 5 > i && i <= particles->size(); i++)
	{
		particle = &(particles)->at(i);
		if(particle->center_mass_y_normalized > topTarget->center_mass_y_normalized)
		{
			topTarget = &(particles)->at(i);//add criteria for too small particles
			topTargetParticle = i;
		}
		delete particle;
	}

	for (unsigned int i = 1; 5 > i && i <= particles->size(); i++)
	{
		particle = &(particles)->at(i);
		if(particle->center_mass_y_normalized < bottomTarget->center_mass_y_normalized)
		{
			bottomTarget = &(particles)->at(i);//add criteria for too small particles
			bottomTargetParticle = i;
		}
		delete particle;
	}


	numberOfTargets = particles->size();//filters order by height requ so above for loops need not remove particles
	
	ParticleAnalysisReport *par = &(particles)->at(topTargetParticle);
	centerX = par->center_mass_x_normalized;
	bottomCenterY = par->center_mass_y_normalized;
	
	par = &(particles)->at(bottomTargetParticle);
	particleHeight = par->boundingRect.height;//gehto divide for h in pix
	height = par->imageHeight;
	topCenterY = par->center_mass_y_normalized;


	if (numberOfTargets < 3)
		limitedDistanceFunction = 1;
	else
		limitedDistanceFunction = 0;
	
	getDistance(approximateDistance);

	delete topTarget;
	delete bottomTarget;
	
	//SendVisionData(*reports);

	delete particles;
}

//////////////////////////////////////////////////////////////////////////////////

/*
void SendVisionData(vector<ParticleAnalysisReport> &kitten) 
{
	Dashboard &d = DriverStation::GetInstance()->GetHighPriorityDashboardPacker();
	int n = (numberOfTargets <= 10) ? numberOfTargets : 10;
	d.AddCluster(); 
	{
		d.AddArray(); 
		{
		for (int i = 0; i < n; i++) //ParticleAnalysisReport *r = &(kitten->at(i));
		{
			ParticleAnalysisReport cat = (kitten.at(i));
				d.AddCluster(); 
				{
					d.AddI16((INT16)cat.center_mass_y-10);
					d.AddI16((INT16)cat.center_mass_x-10);
					d.AddI16((INT16)cat.center_mass_y+10);
					d.AddI16((INT16)cat.center_mass_x+10);
				}
				d.FinalizeCluster();
			}
		}
		d.FinalizeArray();
	}
	d.FinalizeCluster();
	d.Finalize();
}
*/


double CATS::fOfX(double x)//part of accurate distance finding in getDistance()
{
	return (atan((TOP_TARGET-CAMERA_HEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
	+atan((CAMERA_HEIGHT-BOTTOM_TARGET)/(x))); //angle from bottom triangle
}


double CATS::getDistance(double &approximation)//find distance
{
	if (limitedDistanceFunction)//if must use less accurate tracking
	{
		approximation = INCHES_TARGET_HEIGHT/2//half height of target in inches over target to get adjacent
				/tan(//tan of this to get opposite over adjacent
						CAMERA_ANGLE_OF_VIEW*
							((particleHeight)
									/height)//above divided to get ratio of size
										/2);//to get half of angle and therefore right triangle
	}
	else
	{
		double theta=(topCenterY-bottomCenterY)/240*CAMERA_ANGLE_OF_VIEW;//the distance is turned into an angle (refer to fofx(x))
		//107.5 is top target height 31.5 is bottom target height
		
		double dotbinary=54;
		approximation=0;
		for(int i=0; i<30; i++)//binary approximation-> guesses using 1/2 distances until	 //tlar(that looks about right) -- function too complex for now
		{
			dotbinary/=2; //this is the number which modifies the approximation
			if(fOfX(approximation+dotbinary)>theta) //if the value to be added overshoots it does not add
				approximation+=dotbinary;
		}
	}
	return (approximation);
}
