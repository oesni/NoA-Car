#ifndef LaneDetectHPP
#define LaneDetectHPP

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/tracking.hpp"
#include<opencv2\tracking.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include<deque>

using namespace std;
using namespace cv;

class LaneDetect
{
public:
	Mat currFrame; //stores the upcoming frame
	Mat temp;      //stores intermediate results
	Mat temp2;     //stores the final lane segmentshhh

	int diff, diffL, diffR;
	int laneWidth;
	int diffThreshTop;
	int diffThreshLow;
	int ROIrows;
	int vertical_left;
	int vertical_right;
	int vertical_top;
	int smallLaneArea;
	int longLane;
	int  vanishingPt;
	float maxLaneWidth;
	float rho;
	float theta;
	vector<Vec2f>lines;

	//to store various blob properties
	Mat binary_image; //used for blob removal
	Mat houghImg;
	int minSize;
	int ratio;
	float  contour_area;
	float blob_angle_deg;
	float bounding_width;
	float bounding_length;
	Size2f sz;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RotatedRect rotated_rect;
	float angle;
	Point circle_result;

	////////////////////////////////////////  FUNCTION ///////////////////////////////////////////////
	///////////////////////////////////////           ////////////////////////////////////////////////
	LaneDetect(Mat startFrame);
	void getLane();
	void markLane();
	void setAngle(Point mid_point);
	void blobRemoval();
	void nextFrame(Mat &nxt);
	float getAngle();
	
};//end of class LaneDetect

#endif // !LaneDetectHPP
