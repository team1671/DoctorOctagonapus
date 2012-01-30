/*
 *	Gage Ervin and Emmanuel Lopez		Team 1671 
 *	what happened to the other 7?
 *	historical references: Vriska and Doctaoctagonapus
 */


/*
 * COPILOT CONTROL
 * 1 to turn (hold)
 * 2 to shoot
 * trigers will move arm
 * intake is right thumbstick
 * WHEN SHOOTING ---- IMPORTANT!!!  UNLESS AT LEAST ONE TARGET IS IN SITE OR WE GET A COMPASS LATER DO NOT SHOOT
 * 
 * PILOT
 * car drive, but turns full forward/back at extreme
 * 1 will switch drive
*/
 

/*
 * determine lighting values
 *
 * launch enc for speed ratio
 *
 * make warning if out of range
 * 
*/


#include <iostream>
//for console IO
#include "WPILib.h"

//duh
#include "nivision.h"
//for cams
#include "cmath"

#define IO (DriverStation::GetInstance()->GetEnhancedIO())
//IO from driver station

#define CAMERAHEIGHT 80//inches

AxisCamera &camera = AxisCamera::GetInstance();
//output from cameras to driverstation (so we can see it)
ColorImage image(IMAQ_IMAGE_RGB);
//create an image to buffer pics


class DoctaEight : public SimpleRobot
{
	//declarations
	
	BinaryImage* binImg;
	//make image
	
	
	Encoder LTopEnc, LBotEnc;
	//launch system encoders
	Joystick pilot, copilot;
	//
	CANJaguar lefty, righty, leftyB, rightyB, intake, lift, LTop, LBot, arm;
	//left and right motors, recieve ball, lift ball to launching system, launch system, platform arm
	signed char negate, choiceTarget, distanceTarget;
	//negate for turning drive
	double firstTarget, secondTarget, thirdTarget, decrement;
	//calculated distance to target, to slow motors as aiming

	bool limitedDistance;
	//switch between primary and secondary distance tracking based on number of targets
	
public:
	DoctaEight(void):
		
		//constructs
		
		pilot(1),
		copilot(2),
		//controller(USB port)
		lefty(1),
		righty(2),
		arm(3),
		leftyB(4),
		rightyB(5),
		intake(6),
		lift(7),
		LTop(8),
		LBot(9),
		//jag(CANjag number)
		LTopEnc(1,2),
		LBotEnc(3,4)
		//encoders(AChannel, BChannel)
		{
			binImg = image.ThresholdHSL(120, 250, 0, 30, 130, 230);
			//HSL values (MUST BE FOUND BY EXPERIMENT)
			camera.WriteMaxFPS(30);
			//FPS
			camera.WriteBrightness(100);
			//
			camera.WriteWhiteBalance(AxisCamera::kWhiteBalance_Hold);
			//white balance -- set manually via internet connection
			camera.WriteResolution(AxisCamera::kResolution_320x240);
			//resolution
			camera.WriteColorLevel(10);
			//low color
			camera.WriteCompression(0);
			//lower easier on CRIO and harder on cam


			limitedDistance = 0;
			decrement=1;
			negate=1;
			
			lefty.ChangeControlMode(CANJaguar::kPercentVbus);
			righty.ChangeControlMode(CANJaguar::kPercentVbus);
			leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
			rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
			intake.ChangeControlMode(CANJaguar::kPercentVbus);
			lift.ChangeControlMode(CANJaguar::kPercentVbus);
			LTop.ChangeControlMode(CANJaguar::kPercentVbus);
			LBot.ChangeControlMode(CANJaguar::kPercentVbus);
			//CANJags currently % (-1 to 1)
			
			LTopEnc.Reset();
			LBotEnc.Reset();
			//reset encoders
			LTopEnc.Start();
			LBotEnc.Start();
			//start encoders
		}
	
	void targetSelect(void)//selects target
	{
		camera.GetImage(&image);
		//gets image from cam
		vector<ParticleAnalysisReport>* particles = binImg->GetOrderedParticleAnalysisReports();
		//finds targets
		
		cout << "Number of targets: " << particles->size();

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
	//above selects target
	
	void aim(void)
	{
		choiceTarget = 7;
		//default
		targetSelect();
		//if targetSelect finds a target, below will happen
		
		//find what point motors stop then this should be slightly above
		while (/*decrement > .2 or decrement < -.2 && */ copilot.GetRawButton(1) && choiceTarget != 7)
		{

			targetSelect();
			//select target to shoot at-- will potentially change in while because turning may reveal better
			//targets

			RainbowDash();
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
	
	double fOfX(double x)
	{
		return 	(atan((107.5-CAMERAHEIGHT)/(x)) //angle from top triangle (triangle formed by camera target and line at camera's height)
					+atan((CAMERAHEIGHT-31.5)/(x))); //angle from bottom triangle
	}
	
	double getDistance()
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
	
	void shoot(void)
	{
		
		getDistance();
		//if 0, too close to see target-- set jags low
		
			
		//SHOOT HERE!
	}
	
	
	
	
	void drive(void)
	{				
		if (pilot.GetY() > 0 && pilot.GetX() >= 0)//forward right
		{
			lefty.Set(pilot.GetY() * negate * decrement);//left motors full
			leftyB.Set(pilot.GetY() * negate * decrement);//left motors full
			righty.Set(( pilot.GetY() - pilot.GetX() * 2 ) * negate * decrement);//right motors full dec by twiceX abs X
			rightyB.Set(( pilot.GetY() - pilot.GetX() * 2 ) * negate * decrement);
		}//(so up to x = 0 right rev and when y negative, backward curve)
		else if (pilot.GetY() < 0 && pilot.GetX() > 0)//backward left
		{
			righty.Set(pilot.GetY() * negate * -1 * decrement);
			rightyB.Set(pilot.GetY() * negate * -1 * decrement);
			lefty.Set((pilot.GetY() - pilot.GetX() * 2) * negate * decrement);
			leftyB.Set((pilot.GetY() - pilot.GetX() * 2) * negate * decrement);
		}
		else if (pilot.GetY() > 0 && pilot.GetX() <= 0)//forward left
		{
			righty.Set(pilot.GetY() * negate * decrement);
			rightyB.Set(pilot.GetY() * negate * decrement);
			lefty.Set((pilot.GetX() * 2 + pilot.GetY()) * negate * decrement);
			leftyB.Set((pilot.GetX() * 2 + pilot.GetY()) * negate * decrement);
		}
		else if (pilot.GetY() < 0 && pilot.GetX() < 0)//back right
		{
			lefty.Set(pilot.GetY() * negate * -1 * decrement);//left full back
			leftyB.Set(pilot.GetY() * negate * -1 * decrement);//left full back
			righty.Set(( pilot.GetY() + pilot.GetX() * 2 ) * negate * decrement);//right morots full dec by twice abs X
			rightyB.Set(( pilot.GetY() + pilot.GetX() * 2 ) * negate * decrement);
		}
		else
		{
			righty.Set(pilot.GetY() *negate * decrement);
			rightyB.Set(pilot.GetY() *negate * decrement);
			lefty.Set(pilot.GetY() *negate * decrement);
			leftyB.Set(pilot.GetY() *negate * decrement);
		}
	}
	
	
	
	void RainbowDash(void)//pony works like c code braces, like this rainbow.Add<Typegoeshere>(variable)
	{
		Dashboard &rainbow = DriverStation::GetInstance()->GetHighPriorityDashboardPacker();
		rainbow.AddCluster();						//displays the target nubmers
			rainbow.AddDouble(firstTarget);
			rainbow.AddDouble(secondTarget);
			rainbow.AddDouble(thirdTarget);
		rainbow.FinalizeCluster();
		rainbow.AddCluster();						/////displays the distance from the target, and
			rainbow.AddDouble(getDistance());			//and the diffrence number for angling
			rainbow.AddDouble(decrement);
		rainbow.FinalizeCluster();
		rainbow.Finalize();//need this for the ending
	}
	
	
	
	
	void Autonomous(void)
	{
		GetWatchdog().Kill();
		while (IsAutonomous())
		{
			aim();
			shoot();
			//aim and shoot
		}
	}
	
	
	
	
	void OperatorControl(void)
	{
		GetWatchdog().Kill();
		while (IsOperatorControl())
		{
			RainbowDash();
			
			if (copilot.GetTop())
				arm.Set(-1);
			else if (copilot.GetTrigger())
				arm.Set(1);
			else
				arm.Set(0);
			//move arm
			
			
			if (copilot.GetRawButton(1))
			{
				aim();
				//aim
			}
			else
				decrement = 1;
			//if not aiming and shooting, return motor power
			
			if (copilot.GetRawButton(2))
				shoot();
			
			intake.Set(copilot.GetTwist());
			//take the balls

			
			if (pilot.GetRawButton(1)){negate *= -1;}
			//to reverse drive
			
			drive();//drive system
			
		}
		LTopEnc.Stop();
		LBotEnc.Stop();
		//stops encoders
	}
};

START_ROBOT_CLASS(DoctaEight);
//called by driverstation
