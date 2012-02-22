#ifndef CATS_H_
#define CATS_H_

#include "WPILib.h"
#include "Cmath"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"

#define RED_LOW 0
#define RED_HIGH 81
#define GREEN_LOW 0
#define GREEN_HIGH 98
#define BLUE_LOW 188
#define BLUE_HIGH 255

#define FIRST_CRITERIA IMAQ_MT_BOUNDING_RECT_HEIGHT
#define LOWER_AREA_COMPARRISON_CRITERIA 0
#define HIGHER_AREA_COMPARRISON_CRITERIA 90
#define PERCENT_CALIBRATED false
#define PERCENT_REMOVE_OR_KEEP false

#define CAMERA_HEIGHT 49//in
#define TOP_TARGET 107.5
#define INCHES_TARGET_HEIGHT 18
#define BOTTOM_TARGET 31.5
#define CAMERA_ANGLE_OF_VIEW 54

#define SECOND_CRITERIA IMAQ_MT_AREA_BY_PARTICLE_AND_HOLES_AREA
#define LOWER_HEIGHT_CRITERIA 0
#define HIGHER_HEIGHT_CRITERIA 25
#define HEIGHT_CALIBRATED false
#define HEIGHT_REMOVE_OR_KEEP true//false is keep
#define FILTER_NUMBER 2

#define BRIGHTNESS	40
#define COLOR_LEVEL	100
#define COMPRESSION	0
#define MAXIMUM_FPS	5
#define IMAGE_SIZE AxisCamera::kResolution_640x480

class CATS
{
private:
	void GetProcessedImage(void);
	void GetValues(void);
	void SendVisionData(vector<ParticleAnalysisReport> &kitten);
	double fOfX(double x);
	double getDistance(double &approximation);
	Threshold thresholdRGBValues;
	ParticleFilterCriteria2 *criteria;
	AxisCamera *camera;
	BinaryImage *FinalParticles;
	signed char topTargetParticle, bottomTargetParticle;
	double particleHeight, height, bottomCenterY, topCenterY;
	bool limitedDistanceFunction;
public:
	CATS(void);
	void RunAll(void);//will return everything
	
	signed char numberOfTargets;//send these
	double approximateDistance, centerX;
};

#endif
