#include "RectangleDetection.h"


RectangleDetection::RectangleDetection()
{
}

RectangleDetection::~RectangleDetection()
{
}

// 検出して赤線で囲む
cv::Mat RectangleDetection::Detect(cv::Mat frame)
{
	// グレースケールに変換して2値化(一度ある程度の値を飛ばして机の白さを調整してる)
	cv::Mat gray;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, gray, 200, 255, cv::THRESH_TOZERO_INV);
	cv::bitwise_not(gray, gray); // 白黒の反転
	cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 矩形の検出
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(gray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);

	std::vector<std::vector<cv::Point>> tmpContours;
	
	for (int i = 0; i < contours.size(); i++) {
		// ある程度の面積が有るものだけに絞る
		double a = contourArea(contours[i], false);
		if (a > 15000) {
			//輪郭を直線近似する
			std::vector<cv::Point> approx;
			cv::approxPolyDP(cv::Mat(contours[i]), approx, 0.01 * cv::arcLength(contours[i], true), true);
			// 矩形のみ取得
			if (approx.size() == 4) {
				tmpContours.push_back(approx);
			}
		}
	}

	int max_level = 0;
	for (int i = 0; i < tmpContours.size(); i++)
	{
		cv::drawContours(frame, tmpContours, i, cv::Scalar(0, 255, 0, 255), 3, cv::LINE_AA, hierarchy, max_level);
	}

	return frame;
}