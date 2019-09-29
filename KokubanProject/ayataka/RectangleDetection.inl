#include "RectangleDetection.h"


RectangleDetection::RectangleDetection()
{
}

RectangleDetection::~RectangleDetection()
{
}

// ���o���ĐԐ��ň͂�
cv::Mat RectangleDetection::Detect(cv::Mat frame)
{
	// �O���[�X�P�[���ɕϊ�����2�l��(��x������x�̒l���΂��Ċ��̔����𒲐����Ă�)
	cv::Mat gray;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, gray, 200, 255, cv::THRESH_TOZERO_INV);
	cv::bitwise_not(gray, gray); // �����̔��]
	cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// ��`�̌��o
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(gray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);

	std::vector<std::vector<cv::Point>> tmpContours;
	
	for (int i = 0; i < contours.size(); i++) {
		// ������x�̖ʐς��L����̂����ɍi��
		double a = contourArea(contours[i], false);
		if (a > 15000) {
			//�֊s�𒼐��ߎ�����
			std::vector<cv::Point> approx;
			cv::approxPolyDP(cv::Mat(contours[i]), approx, 0.01 * cv::arcLength(contours[i], true), true);
			// ��`�̂ݎ擾
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