DoctaEight::DoctaEight(void):

//constructs

pilot(1),
copilot(2),
//controller(USB port)
lefty(2),
righty(3),
leftyB(4),
rightyB(5),
arm(9),
intake(8),
LTop(6),
LBot(7),
//jag(CANjag number)
LTopEnc(1,2),
LBotEnc(3,4)
//encoders(AChannel, BChannel)
{
	binImg = image.ThresholdHSL(120, 250, 0, 30, 130, 230);
	//HSL values (MUST BE FOUND BY EXPERIMENT)
	camera.WriteMaxFPS(30);
	//FPS
	camera.WriteBrightness(30);
	//
	camera.WriteWhiteBalance(AxisCamera::kWhiteBalance_Hold);
	//white balance -- set manually via internet connection w/hold
	camera.WriteResolution(AxisCamera::kResolution_320x240);
	//resolution
	camera.WriteColorLevel(30);
	//low color
	camera.WriteCompression(30);
	//lower easier on CRIO and harder on cam


	limitedDistance = 0;
	decrement=1;
	negate=1;
	
	lefty.ChangeControlMode(CANJaguar::kPercentVbus);
	righty.ChangeControlMode(CANJaguar::kPercentVbus);
	leftyB.ChangeControlMode(CANJaguar::kPercentVbus);
	rightyB.ChangeControlMode(CANJaguar::kPercentVbus);
	intake.ChangeControlMode(CANJaguar::kPercentVbus);
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
