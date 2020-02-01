#pragma once

#include <opencv2\opencv.hpp>

#include "Eraser/EraserManager.h"

std::vector<cv::Point> chalks;
Eraser::EraserManager* eraserManager = nullptr;
cv::Mat inputImage;

void Initialize();
void Update();