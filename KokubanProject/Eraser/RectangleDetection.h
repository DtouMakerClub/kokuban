#pragma once

#include <opencv2\opencv.hpp>

class RectangleDetection
{
public:
	RectangleDetection();
	~RectangleDetection();

	cv::Mat Detect(cv::Mat frame);
private:
	

};