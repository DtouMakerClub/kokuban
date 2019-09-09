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
		std::cout << "Error : failed open camera" << std:: endl;
		return - 1;
	}

	cv::Mat frame; //取得したフレーム
	while (cap.read(frame)) {
		//取得したフレーム対してする処理を書く
		//fn(frame, 128);

		cv::imshow("win", frame);//画像を表示．
		const int key = cv::waitKey(1);
		if (key == 'q') //qボタンが押されたとき
		{
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;
}

/// <summary>
/// 画像を2値化する（仮）
/// </summary>
/// <param name="frame">処理対象のカラー画像</param>
/// <param name="threshold">2値化の閾値</param>
/// <returns>2値化した画像</returns>
cv::Mat color_to_binary(cv::Mat frame, int threshold) {
	if (frame.empty() == true) {
		std::cout << "Error : failed read image" << std::endl;
	}
	else {
		cv::Mat gray_img; //グレースケール画像を入れる
		cv::Mat threshold_img;//2値化した画像を入れる
		cv::cvtColor(frame, gray_img, cv::COLOR_BGR2GRAY); //グレースケールに変換
		cv::threshold(gray_img, threshold_img, threshold, 255, cv::THRESH_BINARY);//2値化画像に変換

		return threshold_img;
	}
}


void test() {
	cv::Mat input_img = cv::imread("sample.png", cv::IMREAD_UNCHANGED);
	if (input_img.empty() == true) {
		// 画像データが読み込めなかったときは終了する
		std::cout << "Error : failed read img" << std::endl;
	}
	else {
		cv::Mat frame = color_to_binary(input_img, 128);
		cv::imshow("binary", frame);//画像を表示
		cv::waitKey(1);
	}
}


int main()
{
	//camera_capture(0,color_to_binary);
	test();
	std::string x;
	std::cin >> x;
	cv::destroyAllWindows();
}
