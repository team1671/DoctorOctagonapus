/*
 * TODO LIST
 * 
 * IN aim() use PID to set cam FPS
 * 
 * RGB VALUES ALWAYS
 * 
 * complete shooter (exp equ)
 * 
 * set camera to allow anonymous viewing to outline target(available in the web console)
 * http://firstforge.wpi.edu/integratio...02&view=markup
 * 
 * cleanup highly messy code, break back into sepparate files, definitions, nonglobal vars
 * 
 * add 524 ramp   in drvie for accel curve
 * 
 */


//kinect drive off (+1/+2 in drive)
//cube input

/*
 * COPILOT CONTROL
 *
 * A to turn (hold)
 *
 * B to shoot
 * 
 * Y to switch drive  <-not in right now
 *
 * trigers will move arm
 *
 * intake is left thumbstick
 * 
 * WHEN SHOOTING ---- IMPORTANT!!!
 * UNLESS AT LEAST ONE TARGET IS IN SITE OR WE GET A COMPASS LATER DO NOT AIM OR SHOOT BECAUSE NOTHING WILL HAPPEN
 */


/*
 * PILOT
 *
 * press 4 to switch between modified arcade, kinect, and tank
 *
 * 1 will switch drive
*/


#include "WPILib.h"
#include "KinectStick.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "cmath"

#define Kill GetWatchdog().Kill();
#define CAMERAHEIGHT 65
#define ANGLEOFLAUNCH 60
#define VRAMPRATE .3
#define angle 54
#define RL 200
#define RH 260
#define GL 140
#define GH 210
#define BL 90
#define BH 135
#define MAX 2750
#define KP 0.250
#define KI 0.015
#define KD 0.020
#define ENCCOUNT 360
static AxisCamera &eyeOfSauron = AxisCamera::GetInstance("10.16.71.11");

DriverStationLCD* driverOut = DriverStationLCD::GetInstance();

class DoctaEight : public SimpleRobot
{	

	BinaryImage *thresholdImage;
	BinaryImage *bigObjectsImage;
	BinaryImage *convexHullImage;
	BinaryImage *filteredImage;
	vector<ParticleAnalysisReport> *particles;

	Joystick pilot, copilot;
	//	KinectStick *leftArm, *rightArm;
		
	CANJaguar lefty, righty, leftyB, rightyB, intake, arm, LTop, LBot;
	//left and right motors, recieve ball, lift ball to launching system, launch system, platform arm

	
	signed char distanceTarget, choiceTarget, negate, drive;
	//negate for turning drive, choice target target selected, distance target for getting distance, itt for itterations
	
	double firstTarget, secondTarget, thirdTarget;
	//targets (fourth is not one two or three XD), decriment to slow motors as aiming

	bool limitedDistance, cycle, flag, aimin, shootin;
	//switch between primary and secondary distance tracking based on number of targets
	struct CamDataStruct//group for camera data use UpdateCamData before using these
	{
		int numTargets;
		double toCenter, 
			centerY, 
				centerX, 
					firstY, 
						secondY, 
							thirdY, 
								fourthY, 
									area, 
										height, 
											distanceYnorm; //stairway to heaven
	}CamData;
public:
	DoctaEight::DoctaEight(void):

	pilot(1),
	copilot(2),
	//controller(USB port)
	
	lefty(4),
	leftyB(5),
	righty(2),
	rightyB(3),
	
	arm(9),
	intake(8),
	LTop(7),
	LBot(6)
	//controller(USB port)
	//encoders(AChannel, BChannel)
	{
		Kill;
		shootin = 0;
		negate = 0;
		drive =1;
		
		lefty.ChangeControlMode(CANJaguar::kPercentVbus);
		righty.ChangeControlMode(CANJaguar::kPercentVbus);
		leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
		rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
		intake.ChangeControlMode(CANJaguar::kPercentVbus);
		arm.ChangeControlMode(CANJaguar::kPercentVbus);
		
		LTop.ChangeControlMode(CANJaguar::kSpeed);
		LBot.ChangeControlMode(CANJaguar::kSpeed);
		LBot.ConfigEncoderCodesPerRev(ENCCOUNT);
		LTop.ConfigEncoderCodesPerRev(ENCCOUNT);
		//CANJags currently % (-1 to 1)
		LBot.SetPID(KP,KI,KD);
		LTop.SetPID(KP,KI,KD);
		
		lefty.SetVoltageRampRate(VRAMPRATE);
		leftyB.SetVoltageRampRate(VRAMPRATE);
		righty.SetVoltageRampRate(VRAMPRATE);
		rightyB.SetVoltageRampRate(VRAMPRATE);
		LTop.SetVoltageRampRate(VRAMPRATE);
		LBot.SetVoltageRampRate(VRAMPRATE);

		//eyeOfSauron.WriteBrightness(30);
		//eyeOfSauron.WriteWhiteBalance(AxisCamera::kWhiteBalance_Hold);
		//eyeOfSauron.WriteResolution(AxisCamera::kResolution_640x360);
		//eyeOfSauron.WriteColorLevel(100);
		
		//eyeOfSauron.WriteCompression(30);
		//eyeOfSauron.WriteMaxFPS(5);
		//lower easier on CRIO and harder on cam
		
		
		limitedDistance = 0;
	}

	
	void DoctaEight::Autonomous(void)
	{
		Kill;
		while (IsAutonomous() && IsEnabled())
		{
			output();//funcs called in- will get dist and therefore select target
			aim();//funcs called in- will select terget
			shoot();//funcs called in- will get dist and therefore select target
		}
	}
	void DoctaEight::OperatorControl(void)
	{
		Kill;
		while (IsOperatorControl() && IsEnabled())
		{
			output();//funcs called in- updates cam and therefore gets dist and therefore selects target
			if (copilot.GetRawButton(1))
			{
				aim();//funcs called in- will select target
			}
			if (copilot.GetRawButton(2))
			{
				shoot();
			}
			
			arm.Set(copilot.GetTwist());
			//move simple platform arm
			
			intake.Set(copilot.GetY());

			tardis();//robot drive <-below
		}
	}
	
	void targetSelect(void)//pick the target to shoot at (highest visible) then the one to distance with
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
	
	double fOfX(double x)//part of accurate distance finding in getDistance()
	{
		Kill;
		return 	(atan((107.5-CAMERAHEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
				+atan((CAMERAHEIGHT-31.5)/(x))); //angle from bottom triangle
	}	
	
	void getDistance(double &approximation)//find distance
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
	
	//numTargets, toCenter, centerY, centerX, firstY, secondY, thirdY, fourthY, area, height, distanceYnorm
	void UpdateCamData()
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
		
	void output (void)
	{
		Kill;
				
		UpdateCamData();
		double approximation;
		//GET INFO FROM values()
		
		getDistance(approximation);
		if (IsAutonomous())
			driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Autonomous");
		else if (IsOperatorControl())
			driverOut->Printf(DriverStationLCD::kUser_Line1, 1, "Operator");
		
		if (approximation != -1)
			driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "Distance: %f", (double)approximation);
		else if (aimin)
			driverOut->Printf(DriverStationLCD::kUser_Line3, 1, "Double to 0: %f", CamData.centerX);
		
		
		if (choiceTarget == -1)
			driverOut->Printf(DriverStationLCD::kUser_Line2, 3, "no target");
		else if (choiceTarget > 4)
			driverOut->Printf(DriverStationLCD::kUser_Line2, 3, "too many targets");
		else
		{
			driverOut->Printf(DriverStationLCD::kUser_Line4, 3, "Number of targets: %d", CamData.numTargets);
			driverOut->Printf(DriverStationLCD::kUser_Line5, 4, "Target selected: %d", choiceTarget);
		}
		driverOut->UpdateLCD();
	}
	
	void aim()
	{
		Kill;
		//robot width-33			//inches
		//robot length-18			//inches
		
		aimin = 1;
		while (copilot.GetRawButton(1) or IsAutonomous())
		{
			Kill;
			output();
			double decrement = 1;
			double n;
			if (choiceTarget!=-1 and particles->size() < 5)//if there is a target
			{
				while (IsAutonomous() or copilot.GetRawButton(1))
				{
					UpdateCamData();

					if (CamData.centerX > -.2 or CamData.centerX < .2)
					{
						decrement = n*CamData.centerX*CamData.centerX*CamData.centerX;
					}
					if (CamData.centerX < 0)//right
						leftyrighty(decrement, -decrement);
					else if (CamData.centerX > 0)//left
						leftyrighty(-decrement, decrement);
					if (CamData.centerX > -.5 or CamData.centerX < .5)
					{
						leftyrighty(0, 0);
						return;
					}
					if (lefty.GetSpeed() < 0 or righty.GetSpeed() < 0)
						n++;
				}
			}
		}
		aimin = 0;
	}
	
	void shoot(void)
	{
		Kill;
		shootin =1;
		double approximation = 0;
		while (approximation != -1 && copilot.GetRawButton(2))//while in aim() should count also
		{
			getDistance(approximation);
			/*
			 * approximation
			 * CAMERAHEIGHT
			 * ANGLEOFLAUNCH
			 * 
			 * double X = [(V^2)/g * (Squrt(3)/2)]+23.09in
			 * 
			 * setjags(returns of F(X))//F() exp function
			 * 
			 * while (encoders show motors below speed)
			 * 		speed motors
			 * 
			 * remember not need select target as will in get dist
			 */
		}
		//SHOOT HERE!
		shootin = 0;
	}


	
	void DoctaEight::leftyrighty(double left, double right)//set drive motors on either side
	{
		Kill;
		righty.Set(right);
		rightyB.Set(right);
		lefty.Set(left);
		leftyB.Set(left);
	}
	
	void DoctaEight::tardis(void)
	{
		Kill;
		if (pilot.GetRawButton(1) && cycle == 0)
		{
			negate *= -1;
			cycle = 1;
		}
		else if (pilot.GetRawButton(4) && cycle == 0)
		{
			drive = drive%4 + 2;//	Change to plus 1 for kinect drive be included
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
			leftyrighty(negate*pilot.GetY(), negate*pilot.GetRawAxis(4));
		}
	}
		
};
START_ROBOT_CLASS(DoctaEight);
