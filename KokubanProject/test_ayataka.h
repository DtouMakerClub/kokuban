#pragma once

#include <opencv2\opencv.hpp>

#include "Eraser/EraserManager.h"

class test_ayataka
{
public:
	test_ayataka();
	~test_ayataka();

	void exec(cv::Mat img);
	void drawTest(cv::Mat img);

	Eraser::EraserManager* eraserManager = nullptr;

private:
	cv::Point2i pos;					// ���ݒn
	int area;							// �G���A
	std::vector<cv::Point2i> chalks;
};