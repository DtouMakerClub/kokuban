#include <opencv2/opencv.hpp>

///<summary>
///<para>カメラと接続してフレームごとに処理をする</para>
/// <para>camera open and some process every frame</para>
///</summary>
///<param name="device_nan">開くデバイスの選択</param>
/// <param name="fn">取得したフレームを引数に取り処理をする関数</param>
template<class Fn> int camera_capture(int device_nan, Fn fn) {
	assert(device_nan >= 0);
	cv::VideoCapture cap(device_nan);
	if (!cap.isOpened()) {
		std::cout << "Error : failed Open camera" << std:: endl;
		return - 1;
	}

	cv::Mat frame; //取得したフレーム
	while (cap.read(frame)) {
		//取得したフレーム対してする処理を書く
		//fn(frame);
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;
}

void test() {
	std::cout << "test" << std::endl;
}


int main()
{
	camera_capture(0,test);
	std::string x;
	std::cin >> x;
}
