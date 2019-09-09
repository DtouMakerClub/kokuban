#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
	Mat image = Mat::zeros(100, 100, CV_8UC3);
	imshow("", image);
	waitKey(1);
	String x;
	std::cin >> x;
}
