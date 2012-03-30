#ifndef CAMERA_H_
#define CAMERA_H_

#include "WPILib.h"
#include "Cmath"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"

#define PI 3.1425926558979323846264

#define RED_LOW 16
#define RED_HIGH 119
#define GREEN_LOW 10
#define GREEN_HIGH 186
#define BLUE_LOW 200
#define BLUE_HIGH 255

#define FIRST_CRITERIA IMAQ_MT_BOUNDING_RECT_HEIGHT
#define PERCENT_CALIBRATED false
#define PERCENT_REMOVE_OR_KEEP false

#define SHOOTER_HEIGHT 2.56*51/100
#define TOP_TARGET 107.5
#define INCHES_TARGET_HEIGHT 18
#define BOTTOM_TARGET 31.5
#define CAMERA_ANGLE_OF_VIEW 54/180*PI//in radians

#define SECOND_CRITERIA IMAQ_MT_AREA_BY_PARTICLE_AND_HOLES_AREA
#define LOWER_HEIGHT_CRITERIA 0
#define HIGHER_HEIGHT_CRITERIA 25
#define HEIGHT_CALIBRATED false
#define HEIGHT_REMOVE_OR_KEEP true//false is keep
#define FILTER_NUMBER 2

#define BRIGHTNESS	50
#define COLOR_LEVEL	50
#define COMPRESSION	0
#define MAXIMUM_FPS	5

#define THETA 60*PI/180
#define g -9.8//m/s^2

#define IMAGE_SIZE AxisCamera::kResolution_640x480
//#define INCHES_TO_METERS 2.54/100

class Camera
{
private:
	void GetProcessedImageVals(void);
	void SendVisionData(vector<ParticleAnalysisReport> &kitten);
	double fOfX(double x);
	double getDistance(double &approximation);
	void QualitativeOrder(void);
	void YOrganizer(void);
	void SetVals(void);
	void FilterAndAnal(void);
	void SelectDistancing(void);
	
	Threshold thresholdRGBValues;
	ParticleFilterCriteria2 *criteria;
	BinaryImage *FinalParticles;
	AxisCamera *Cam;
	signed char topTargetParticle, bottomTargetParticle;
	double bottomParticleHeight, topParticleHeight, bottomCenterY, topCenterY, imageHeight, approximateDistance, targetHeight;
	bool limitedDistanceFunction;
	ColorImage *img;
	BinaryImage *thresholdImage;
	BinaryImage *filteredImage;
	BinaryImage *biFilteredImage;
	BinaryImage *convexHullImage;
	vector<ParticleAnalysisReport> *reports;
	
public:
	Camera(void);
	void RunAll(void);//will return everything
	
	signed char numberOfTargets;//send these
	double centerX, velocity;
};

#endif
