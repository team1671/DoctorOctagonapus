#include"Vrisk.h"

void DoctaEight::UpdateCamData()
{

	//ParticleFilterCriteria2 criteria[] = {
	//									{IMAQ_MT_BOUNDING_RECT_WIDTH, 10, 200, false, false},
	//									{IMAQ_MT_BOUNDING_RECT_HEIGHT, 10, 200, false, false}};
	ColorImage image(IMAQ_IMAGE_RGB);
	if (eyeOfSauron.IsFreshImage())//if latest from cam not recieved
	{
		eyeOfSauron.GetImage(&image);
	}
	else
	{
		/*
		Image* BCG = image.GetImaqImage();
		delete &image;
		BCGOptions bcg;
		bcg.brightness = BS;
		bcg.contrast = CT;
		bcg.gamma = GA;
		imaqBCGTransform(BCG, BCG, &bcg, NULL);
		//note-can use above to sep particles then get rid of smaller than target size at furthest range
		HSLImage *thresholdLUM = BCG->ThresholdHSL(1,2,1,2,1,2);
		delete &BCG;
		MonoImage* luminance = thresholdLUM->GetLuminancePlane();
		 delete thresholdLUM;
		imaqDispose(BCG);
		image.ReplaceRedPlane(luminance);
		image.ReplaceGreenPlane(luminance);
		image.ReplaceBluePlane(luminance);
		BinaryImage *thresholdImage = BCG->ThresholdRGB(threshold);
		BinaryImage *binImg = luminance;
			delete luminance;
		 imaqConvexHull(binImg->GetImaqImage(),binImg->GetImaqImage(),1);
		BinaryImage *bigObjectsImage = binImg->RemoveSmallObjects(false, 2);
		delete binImg;
		BinaryImage *convexHullImage = bigObjectsImage->ConvexHull(false);
		delete bigObjectsImage;
		BinaryImage *filteredImage = convexHullImage->ParticleFilter(criteria, 2);
		delete convexHullImage;*/

		static Threshold threshold(HUL,HUH,SAL,SAH,LUL,LUH);
		BinaryImage *thresholdHSL = image.ThresholdHSL(threshold);
		vector<ParticleAnalysisReport> *particles = thresholdHSL ->GetOrderedParticleAnalysisReports();
		delete thresholdHSL;
		
		
		ParticleAnalysisReport *high;
		ParticleAnalysisReport *particle;
		ParticleAnalysisReport *low;
		for (unsigned int i = 1; i <= particles->size(); i++)
		{
			particle = &(particles)->at(i);
			if(particle->center_mass_y_normalized > high->center_mass_y_normalized)
			{
				high = &(particles)->at(i);//add criteria for too small particles
				CamData.HP = i;
			}
			delete particle;
		}
		
		for (unsigned int i = 1; i <= particles->size(); i++)
		{
			particle = &(particles)->at(i);
			if(particle->center_mass_y_normalized < low->center_mass_y_normalized)
			{
				low = &(particles)->at(i);//add criteria for too small particles
				CamData.LP = i;
			}
			delete particle;
		}
		
		//aim at target or whatev			
		CamData.numTargets = particles->size();//MAKE SURE ALL ARE VALID SIZE RANGE IN ABOVE FILTERS
		ParticleAnalysisReport *par = &(particles)->at(CamData.HP);
		CamData.numTargets = particles->size();
		CamData.centerX = par->center_mass_x_normalized;
		CamData.centerY = par->center_mass_y_normalized;
		par = &(particles)->at(CamData.LP);
		CamData.area = par->particleArea;
		CamData.height = par->imageHeight;
		CamData.distanceYnorm = par->center_mass_y_normalized;
		
		double app;
		if (CamData.numTargets > 0)
			CamData.approx = getDistance(app);
		else
			CamData.approx = -1;
		
		if (IsAutonomous())
		{
			aim();//funcs called in- will select terget
			shoot();//funcs called in- will get dist and therefore select target
		}
		else
		{
			if (copilot.GetRawButton(1))
			{
				aim();//funcs called in- will select target
			}
			if (copilot.GetRawButton(2))
			{
				shoot();
			}
		}
		
		output();
		
		delete particles;
		delete high;
		delete low;
	}

}



double DoctaEight::fOfX(double x)//part of accurate distance finding in getDistance()
{
	Kill;
	return 	(atan((TOPTARGET-CAMERAHEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
			+atan((CAMERAHEIGHT-BOTTOMTARGET)/(x))); //angle from bottom triangle
}	

double DoctaEight::getDistance(double &approximation)//find distance
{
	Kill;
	if (CamData.numTargets < 3)//if must use less accurate tracking
	{
		approximation = 9//half height of target in inches over target to get adjacent
						/tan(//tan of this to get opposite over adjacent
								CAMANGLE*
									((CamData.area/24)//to get height in pixels
										/CamData.height)//above divided to get ratio of size
											/2);//to get half of angle and therefore right triangle
	}
	else
	{
		double theta=(CamData.centerY-CamData.distanceYnorm)/240*CAMANGLE;//the distance is turned into an angle (refer to fofx(x))
		//107.5 is top target height 31.5 is bottom target height
		
		double dotbinary=54;
		approximation=0;
		for(int i=0; i<30; i++)//binary approximation-> guesses using 1/2 distances until							//tlar(that looks about right) -- function too complex for now
		{
			dotbinary/=2; //this is the number which modifies the approximation
			if(fOfX(approximation+dotbinary)>theta) //if the value to be added overshoots it does not add
				approximation+=dotbinary;
		}
	}
	return (approximation);
}

void DoctaEight::setCam(void)
{
	eyeOfSauron.WriteBrightness(30);
	eyeOfSauron.WriteWhiteBalance(AxisCamera::kWhiteBalance_Hold);
	eyeOfSauron.WriteResolution(AxisCamera::kResolution_320x240);
	eyeOfSauron.WriteColorLevel(100);
	eyeOfSauron.WriteCompression(0);
	//lower easier on CRIO and harder on cam
	eyeOfSauron.WriteMaxFPS(5);
}
