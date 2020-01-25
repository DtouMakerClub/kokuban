#include <opencv2/opencv.hpp>

#include "test_ayataka.h"

test_ayataka::test_ayataka()
{
	eraserManager = new Eraser::EraserManager();
}

test_ayataka::~test_ayataka()
{
	delete(eraserManager);
}

void test_ayataka::exec(cv::Mat img)
{
	eraserManager->Update();

	pos = eraserManager->GetEraserPos();
	area = eraserManager->GetAreaIndex();
	chalks = eraserManager->GetChalk();

	drawTest(img);
}

void test_ayataka::drawTest(cv::Mat img)
{
	//img = cv::Scalar(255, 255, 255);

	// チョークリーナー
	cv::Point p1(20, 20);
	cv::rectangle(img, pos, pos + p1, cv::Scalar(255, 0, 0), 1, cv::LINE_4);

	// チョーク
	//for (auto chalk : chalks)
	//{
	//	cv::rectangle(img, chalk, chalk + p1, cv::Scalar(0, 0, 0), 1, cv::LINE_4);
	//}

	cv::imshow("test", img);
	cv::waitKey(1);
}