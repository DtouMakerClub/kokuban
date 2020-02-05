#pragma once

#include <opencv2\opencv.hpp>

#include "Eraser/EraserManager.h"

cv::Point2f src_pt[4] = {};
std::vector<cv::Point> chalks;
Eraser::EraserManager* eraserManager = nullptr;
cv::Mat inputImage;
cv::VideoCapture cap;

void Initialize();
void Update();