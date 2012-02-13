#include"Doc8.h"

void DoctaEight::UpdateCamData()
{
	Kill;
	Threshold thresholdRGB(RL,RH,GL,GH,BL,BH);
	ParticleFilterCriteria2 criteria[] = {{IMAQ_MT_BOUNDING_RECT_WIDTH, 30, 400, false, false},
										{IMAQ_MT_BOUNDING_RECT_HEIGHT, 40, 400, false, false}};
	//false Set this element to TRUE to take calibrated measurements.
	//fales Set this element to TRUE to indicate that a match occurs when the measurement is outside the criteria range
	
	ColorImage image(IMAQ_IMAGE_RGB);//make image
	eyeOfSauron.GetImage(&image);//store camera feed to image
	thresholdImage = image.ThresholdRGB(thresholdRGB);//abliteration prep
	bigObjectsImage = thresholdImage->RemoveSmallObjects(false, 2);  // remove small objects (noise)
	convexHullImage = bigObjectsImage->ConvexHull(false);  // fill in partial and full rectangles
	filteredImage = convexHullImage->ParticleFilter(criteria, 2);  // find the rectangles
	particles = filteredImage->GetOrderedParticleAnalysisReports();  // get the results		

		targetSelect();//for choice target
		ParticleAnalysisReport& par = (*particles)[choiceTarget];//get values for centering from selected target
		CamData.numTargets = particles->size();
		CamData.centerX = par.center_mass_x_normalized;
		CamData.centerY = par.center_mass_y_normalized;
		
		par = (*particles)[1];
		CamData.firstY = par.center_mass_y;
		par = (*particles)[2];
		CamData.secondY = par.center_mass_y;
		par = (*particles)[3];
		CamData.thirdY = par.center_mass_y;
		par = (*particles)[4];
		CamData.fourthY = par.center_mass_y;
		
		par = (*particles)[distanceTarget];
		CamData.area = par.particleArea;
		CamData.height = par.imageHeight;
		
		CamData.distanceYnorm = par.center_mass_y_normalized;
	delete &image;
	delete filteredImage;
	delete convexHullImage;
	delete bigObjectsImage;
	delete thresholdImage;
	/*for (char i = 0; i < 3; i++)//IF THIS VERSION USED, NEED ARRAY FOR CHOICE TARGET
	{//DISTANCE approximation(BUT NEVER EVER CALL GET DIST HERE- JUST NEED THE VAR AN ARRAY), AND EVERY VARRIABLE MADE IN THIS
		Kill;
		int rl,rh,gl,gh,bl,bh, RGB [6] = {rl = RL,rh = RH,gl = GL,gh = GH,bl = BL,bh = BH};
		for (char e=2; e<6; e=e+2)
		{
			Kill;
			RGB[(i+e)%6] = 0;
			RGB[(i+e+1)%6] = 255;
		}
		Threshold thresholdRGB(RGB[0],RGB[1],RGB[2],RGB[3],RGB[4],RGB[5]);
		ParticleFilterCriteria2 criteria[] = {{IMAQ_MT_BOUNDING_RECT_WIDTH, 30, 400, false, false},
											{IMAQ_MT_BOUNDING_RECT_HEIGHT, 40, 400, false, false}};
		//false Set this element to TRUE to take calibrated measurements.
		//fales Set this element to TRUE to indicate that a match occurs when the measurement is outside the criteria range
		
		ColorImage image(IMAQ_IMAGE_RGB);//make image
		eyeOfSauron.GetImage(&image);//store camera feed to image
		thresholdImage = image.ThresholdRGB(thresholdRGB);//abliteration prep
		bigObjectsImage = thresholdImage->RemoveSmallObjects(false, 2);  // remove small objects (noise)
		convexHullImage = bigObjectsImage->ConvexHull(false);  // fill in partial and full rectangles
		filteredImage = convexHullImage->ParticleFilter(criteria, 2);  // find the rectangles
		particles = filteredImage->GetOrderedParticleAnalysisReports();  // get the results		

			targetSelect();
			ParticleAnalysisReport& par = (*particles)[choiceTarget];//get values for centering from selected target
			CamData.numTargets = particles->size();
			CamData.centerX = par.center_mass_x_normalized;
			CamData.centerY = par.center_mass_y_normalized;
			
			par = (*particles)[1];
			CamData.firstY = par.center_mass_y;
			par = (*particles)[2];
			CamData.secondY = par.center_mass_y;
			par = (*particles)[3];
			CamData.thirdY = par.center_mass_y;
			par = (*particles)[4];
			CamData.fourthY = par.center_mass_y;
			
			par = (*particles)[distanceTarget];
			CamData.area = par.particleArea;
			CamData.height = par.imageHeight;
			
			CamData.distanceYnorm = par.center_mass_y_normalized;
		delete &image;
		delete filteredImage;
		delete convexHullImage;
		delete bigObjectsImage;
		delete thresholdImage;
	}*/
}

void DoctaEight::targetSelect(void)//pick the target to shoot at (highest visible) then the one to distance with
{
	Kill;
			
	if (3 == CamData.numTargets or 4 == CamData.numTargets)//if 3 or 4 targets visible
	{
		//choose target to aim/shoot at -> highest target
		if (CamData.firstY > CamData.secondY && CamData.firstY > CamData.thirdY)
			choiceTarget = 1;
		else if (CamData.secondY > CamData.thirdY && CamData.secondY > CamData.firstY)
			choiceTarget = 2;
		else if (CamData.thirdY > CamData.secondY && CamData.thirdY > CamData.firstY)
			choiceTarget = 3;
		else {choiceTarget = 4;}

		//find lowest target for distance tracking purposes
		if (CamData.firstY < CamData.secondY && CamData.firstY < CamData.thirdY)
			distanceTarget = 1;
		else if (CamData.secondY < CamData.thirdY && CamData.secondY < CamData.firstY)
			distanceTarget = 2;
		else if (CamData.thirdY < CamData.secondY && CamData.thirdY < CamData.firstY)
			distanceTarget = 3;
		else 
			distanceTarget = 4;

		limitedDistance = 0;//switch between high and low accuracy distance tracking (high requires 3 or 4 particles)
	}
	else if (2 == CamData.numTargets)//if 2 targets visible
	{

		//select target to aim/shoot at
		if (CamData.firstY > CamData.secondY)
			choiceTarget = 1;
		else
			choiceTarget = 2;

		limitedDistance = 1;//use single target target tracking
	}
	else if (1 == CamData.numTargets)
	{
		//aim/shoot at only target
		choiceTarget = 1;
		limitedDistance = 1;
	}
	else
		choiceTarget = -1;
}

void DoctaEight::getDistance(double &approximation)//find distance
{
	Kill;
	
	targetSelect();
	
	
	if (choiceTarget !=-1 && CamData.numTargets < 5)//if there is a target
	{
		if (limitedDistance == 1)//if must use less accurate tracking
		{
			approximation = 9//half height of target in inches over target to get adjacent
							/tan(//tan of this to get opposite over adjacent
									angle*
										((CamData.area/24)//to get height in pixels
											/CamData.height)//above divided to get ratio of size
												/2);//to get half of angle and therefore right triangle
		}
		else//better tracking
		{
			
			double theta=(CamData.centerY-CamData.distanceYnorm)/240*angle;//the distance is turned into an angle (refer to fofx(x))
			//107.5 is top target height 31.5 is bottom target height
			
			double dotbinary=54;
			for(int i=0; i<30; i++)//binary approximation-> guesses using 1/2 distances until							//tlar(that looks about right) -- function too complex for now
			{
			dotbinary/=2; //this is the number which modifies the approximation
			if(fOfX(approximation+dotbinary)>theta) //if the value to be added overshoots it does not add
				approximation+=dotbinary;
			}
		}		
		
	}
	if (choiceTarget == -1 or choiceTarget > 4)//if there is no target return -1 to indicate target not found -- must be interpeted in 'shoot'
		approximation= -1;
}

double DoctaEight::fOfX(double x)//part of accurate distance finding in getDistance()
{
	Kill;
	return 	(atan((107.5-CAMERAHEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
			+atan((CAMERAHEIGHT-31.5)/(x))); //angle from bottom triangle
}	
