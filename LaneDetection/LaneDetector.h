#pragma once
#include <opencv2\opencv.hpp>
#include <expvideo\CameraUtility.h>

struct changeConstrast
{
	void operator() (cv::Point3_<uint8_t> &pixel, const int *position) const
	{
		double alpha = 1.5;
		int beta = -100;
		pixel.x = cv::saturate_cast<uchar> (pixel.x*alpha + beta);
		pixel.y = cv::saturate_cast<uchar> (pixel.y*alpha + beta);
		pixel.z = cv::saturate_cast<uchar> (pixel.z*alpha + beta);
	}
};


class LaneDetector
{
public:
	LaneDetector() {};
	~LaneDetector() {};

	
	void constructPerspectiveMapping(cv::Size imgSize);
	// construct look up table
	// @imgSize: input image size
	// @width: output image width defined in LUTs
	// @mppx: meter per pixel in x direction
	// @mppy: meter per pixel in y direction
	void constructLUT(cv::Size imgSize,
		int width, double mppx, double mppy);
	
	// parallel project camera image to ground image
	// @src: camera image
	// @imgG: output ground image
	void getGroundImage(const cv::Mat &src, cv::Mat &imgG);

	void detectLane(const cv::Mat &src, cv::Mat &dst,
		cv::Mat &dst1);


	// define region of interest and this ROI will be 
	// used for the subsequent processing
	// @gray: only take gray image
	static void defineROI(const cv::Mat &gray, cv::Mat &dst);

	// mask the lane color, return the pixel in the 
	// color (white or yellow) range
	static void colorThresholding(const cv::Mat &src, cv::Mat &maskOut);

	// find inner edges
	// @gray: only take gray image
	// @lpts: output points on the left side 
	// @rpts: output points on the right side 
	static bool findInnerEdges(const cv::Mat &gray,
		std::vector<cv::Point> &lpts, std::vector<cv::Point> &rpts);


	// fit line
	// @pts: points 2i
	// @line: outcome line equation vector 3d (a,b,c) => 
	// ax+by+c=0
	static bool fitLine(const std::vector<cv::Point> pts, 
		cv::Vec3d &line);


	cv::Mat mTM_vp; // transformation from vehicle to pixel frame
	
	// camera parameters
	double mFocalX = 0.028; 
	double mFocalY = 0.028;  
	double mCCDX = 0.01586;
	double mCCDY = 0.0132;

	// LUTs
	double mMPPx;
	double mMPPy;
	cv::Mat mXMap, mYMap, mZMap; // double format
	cv::Mat mLUT_u, mLUT_v;  // uchar format

};






