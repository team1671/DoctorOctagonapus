




//change target height based on which distancing
//s-note convert vnot to speed
//try and catch






#include "Camera.h"

Camera::Camera(void):thresholdRGBValues(RED_LOW,RED_HIGH,GREEN_LOW,GREEN_HIGH,BLUE_LOW,BLUE_HIGH)
{
	Cam = &AxisCamera::GetInstance("10.16.71.11");
	ParticleFilterCriteria2 smartass[] =
				{
					FIRST_CRITERIA,
					LOWER_HEIGHT_CRITERIA,
					HIGHER_HEIGHT_CRITERIA,
					HEIGHT_CALIBRATED,
					HEIGHT_REMOVE_OR_KEEP
				};
	criteria = smartass;
	 
	Cam->WriteBrightness(BRIGHTNESS);
	Cam->WriteResolution(IMAGE_SIZE);
	Cam->WriteColorLevel(COLOR_LEVEL);
	Cam->WriteCompression(COMPRESSION);
	//lower easier on CRIO and harder on cam
	
	Cam->WriteMaxFPS(MAXIMUM_FPS);
	Cam->WriteWhiteBalance(AxisCamera::kWhiteBalance_Hold);
	Cam->WriteResolution(AxisCamera::kResolution_320x240);
	targetHeight = 9*12;//inches
	img = NULL;
}

void Camera::RunAll(void)
{
	GetProcessedImageVals();
}

void Camera::GetProcessedImageVals()
{
	if (Cam->IsFreshImage())//if values need updating
	{
		img = Cam->GetImage();
		if (img->GetWidth() > 0)//if calculating things wont cause an exception
		{
			FilterAndAnal();
			if (reports->size() != 0)
			{
				QualitativeOrder();//pick out likely targets	
				YOrganizer();//select targets which are highest and lowest for distancing
				SetVals();//store analysis values to variables
				SelectDistancing();//decide which distancing function to use based on y offset
				//targetHeight
//				getDistance(approximateDistance);inches- >alter this later to use a get speed function... then use those speeds and graph error.. then fix
			}
			else
				numberOfTargets = 0;
		}
	}
}


void Camera::FilterAndAnal()
{
	//filters
	thresholdImage = img->ThresholdRGB(thresholdRGBValues);	// get just the red target pixels
	filteredImage = thresholdImage->ParticleFilter(criteria, 1);  // find the rectangles
	convexHullImage = filteredImage->ConvexHull(false);  // fill in partial and full rectangles

	//make analysis
	reports = convexHullImage->GetOrderedParticleAnalysisReports();  // get the results
}

void Camera::QualitativeOrder()
{
	for (unsigned int i = 0; i < reports->size(); i++)
	{
		for (unsigned int b = i; b < reports->size(); b++)
		{
			if ((reports->at(i)).particleQuality > (reports->at(b)).particleQuality)
			{
				swap(reports->at(i),reports->at(b));
			}
		}
	}
}

void Camera::YOrganizer()
{
	//select highest and lowest of top 4 reports
	topTargetParticle=0;
	bottomTargetParticle=0;
	for (unsigned int i = 0; i < reports->size() && i < 4; i++)
	{
		if(reports->at(i).center_mass_y > reports->at(topTargetParticle).center_mass_y)
		{
			topTargetParticle=i;
		}
		else
		{
			bottomTargetParticle=i;
		}
	}
}

void Camera::SetVals()
{
	//set some vals for doin stuff
	numberOfTargets = reports->size();
	centerX = reports->at(topTargetParticle).center_mass_x_normalized;
	topCenterY = reports->at(topTargetParticle).center_mass_y_normalized;
	bottomCenterY = reports->at(bottomTargetParticle).center_mass_y_normalized;
	bottomParticleHeight = reports->at(bottomTargetParticle).boundingRect.height;
	topParticleHeight = reports->at(topTargetParticle).boundingRect.height;
	imageHeight = img->GetHeight();
}

void Camera::SelectDistancing()
{
	if//if i don't have the top and bottom target
	(
		.2 < abs
			(
				(reports->at(topTargetParticle).center_mass_x - reports->at(bottomTargetParticle).center_mass_x)
				/
					(reports->at(topTargetParticle).center_mass_y - reports->at(topTargetParticle).center_mass_y)
						)
							)//stariway to heaven
	{
		limitedDistanceFunction=1;
	}
	else
	{
		limitedDistanceFunction=0;
	}
}

/*
double Camera::fOfX(double x)//part of accurate distance finding in getDistance()
{
	return (atan((TOP_TARGET-SHOOTER_HEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
	+atan((SHOOTER_HEIGHT-BOTTOM_TARGET)/(x))); //angle from bottom triangle
}

double Camera::getDistance(double &approximation)//find distance
{
	if (limitedDistanceFunction)//if must use less accurate tracking
	{
		approximation = INCHES_TARGET_HEIGHT
				/2
					/tan(
						Camera_ANGLE_OF_VIEW*
							bottomParticleHeight
									/imageHeight
										/2
											);		//stairway to hell
	}
	else
	{
		//i is slightly annoyed w/u/eman >.<
		//i is too!
		//nvm- not rly XD
		
		double theta=(topCenterY-bottomCenterY)*Camera_ANGLE_OF_VIEW/2;//the distance is turned into an angle (refer to fofx(x))
		//107.5 is top target height 31.5 is bottom target height
		printf("%f\n",theta);
		Wait(.5);
		double dotbinary=54*12;
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
*/
