#include <opencv2/opencv.hpp>

#include "ayataka/EraserManager.inl"
#include "ayataka/EraserStateBase.inl"

namespace Eraser
{
	class test_ayataka
	{
	public:
		test_ayataka();
		~test_ayataka();

		void exec();
		void drawTest();

		EraserManager* eraserManager = nullptr;

	};

	test_ayataka::test_ayataka()
	{
		eraserManager = new EraserManager();
	}

	test_ayataka::~test_ayataka()
	{
		delete(eraserManager);
	}

	void test_ayataka::exec()
	{
		eraserManager->Update();

		drawTest();
	}

	void test_ayataka::drawTest()
	{
		cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);
	}
}