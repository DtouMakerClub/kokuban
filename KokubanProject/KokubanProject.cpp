#include <opencv2/opencv.hpp>
#include "kokubanCV.h"

#include "KokubanProject.h"

#include "Eraser/EraserManager.inl"

/// <summary>
/// 画像を2値化する（仮）
/// </summary>
/// <param name="frame">処理対象のカラー画像</param>
/// <param name="threshold">2値化の閾値</param>
/// <param name="is_bright">画像が明るいときはtrueにすると良いかも</param>
/// <returns>2値化した画像</returns>
cv::Mat color_to_binary(cv::Mat frame, int threshold = 128, bool is_bright = false) {
	if (frame.empty() == true) {
		std::cout << "Error : failed read image" << std::endl;
	}
	else {
		cv::Mat gray_image; //グレースケール画像を入れる
		cv::Mat threshold_image;//2値化した画像を入れる
		cv::cvtColor(frame, gray_image, cv::COLOR_BGR2GRAY); //グレースケールに変換

		//2値化画像に変換
		if (is_bright) {
			cv::threshold(gray_image, threshold_image, 200, 255, cv::THRESH_TOZERO_INV);
			cv::bitwise_not(threshold_image, threshold_image); // 白黒の反転
			cv::threshold(threshold_image, threshold_image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		}
		else {
			cv::threshold(gray_image, threshold_image, threshold, 255, cv::THRESH_BINARY);
		}

		return threshold_image;
	}
}

/// <summary>
/// 矩形を検出する
/// 検出した矩形に線を描いた画像を返す
/// </summary>
/// <param name="threshold_image">画像</param>
/// <returns>黒板の領域を検出した座標</returns>
std::vector<std::vector<cv::Point>> conto(cv::Mat img ){
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat gray_img;
	int detection_size = 15; // 検出する矩形の大きさ

	gray_img = color_to_binary(img, 128);//2値画像に変換する
	//矩形検出
	cv::findContours(gray_img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);

	int max_level = 0;
	for (int i = 0; i < contours.size(); i++) {
		// ある程度の面積が有るものだけに絞る
		double detection_area = contourArea(contours[i], false);
		//一定以上の面積しか検出しない
		if (detection_area > detection_size) {
			//輪郭を直線近似する
			std::vector<cv::Point> approx;
			cv::approxPolyDP(cv::Mat(contours[i]), approx, 0.01 * cv::arcLength(contours[i], true), true);
			// 矩形のみ取得
			if (approx.size() == 4) {
				cv::drawContours(img, contours, i, cv::Scalar(255, 0, 0, 255), 3, 8, hierarchy, max_level);
				//cv::imshow("binary", img);//画像を表示
			}
		}
	}
	return contours;
}


///<summary>
///<para>カメラと接続してフレームごとに処理をする</para>
/// <para>camera open and some process every frame</para>
///</summary>
///<param name="device_nan">開くデバイスの選択</param>
/// <param name="fn">取得したフレームを引数に取り処理をする関数</param>
template<class Fn> void run_capture_and_process(int device_nan, Fn fn) {
	assert(device_nan >= 0);
	cv::VideoCapture cap(device_nan);
	if (!cap.isOpened()) {
		std::cout << "Error : failed open camera" << std::endl;
		//break;
	}

	cv::Mat frame; //取得したフレーム
	cv::Mat out;
	while (cap.read(frame)) {
		//取得したフレーム対してする処理を書く

		cv::imshow("win", fn(frame, 128,true));//画像を表示．
		const int key = cv::waitKey(1);
		if (key == 'q') //qボタンが押されたとき
		{
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();
}


void test_binary() {
	cv::Mat input_img = cv::imread("sample.png", cv::IMREAD_UNCHANGED);
	if (input_img.empty() == true) {
		// 画像データが読み込めなかったときは終了する
		std::cout << "Error : failed read img" << std::endl;
	}
	else {
		cv::Mat frame = color_to_binary(input_img, 18);
		cv::imshow("binary", frame);//画像を表示
		cv::waitKey(1);
	}
}

void test_Capture() {

	//kokubanCV::funcVideo("kokubanvideo.mp4","kok.mp4", color_to_binary);
	cv::VideoCapture cap = kokubanCV::openVideoFile("kokubanvideo.mp4");
	// 作成する動画ファイルの諸設定
	int    fourcc, width, height;
	double fps;

	width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);	// フレーム横幅を取得
	height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);	// フレーム縦幅を取得
	fps = cap.get(cv::CAP_PROP_FPS);					// フレームレートを取得
	fourcc = (int)cap.get(cv::CAP_PROP_FOURCC);//cv::VideoWriter::fourcc('X', 'V', 'I', 'D');	
	// * エンコード形式 "XVID" = AVI, "MP4V" = MPEG4, "WMV1" = WMV

	// 動画ファイルを書き出すためのオブジェクトを宣言する
	cv::VideoWriter writer;
	writer.open("koku.mp4", fourcc, fps, cv::Size(width, height));

	cv::Mat frame, dst;

	while (true) {

		cap >> frame;

		if (frame.empty() == true) {
			break;
		}

		//何か処理をする
		dst = kokubanCV::binary(frame);

		//cv::imshow("変換中", dst);
		//std::cout << "処理中" << std::endl;
		writer << dst;

		//cv::waitKey(1);
	}
	writer.release();
	std::cout << "end " << std::endl;
	/*
	cv::Mat input_img = cv::imread("kokuban.jpg", cv::IMREAD_UNCHANGED);
	if (input_img.empty() == true) {
		// 画像データが読み込めなかったときは終了する
		std::cout << "Error : failed read img" << std::endl;
	}
	else {
		cv::Mat frame = color_to_binary(input_img, 128);
			//conto(input_img);
		cv::imshow("binary", frame);//画像を表示
		cv::waitKey(1);
	}//*/
}

std::vector<cv::Point> a;

void testChalk() {
	cv::Mat input_img = cv::imread("kokutes.jpg", cv::IMREAD_UNCHANGED);
	if (input_img.empty() == true) {
		// 画像データが読み込めなかったときは終了する
		std::cout << "Error : failed read img" << std::endl;
	}
	else {
		cv::Mat frame = input_img;// color_to_binary(input_img, 128);
		a = kokubanCV::pulledOutChalkOnKokuban(frame);
		cv::imshow("binary", frame);//画像を表示
		cv::waitKey(1);
	}
}


void  testMouse() {
	std::string image_name = "kokuuchi350";
	cv::Mat input_img = cv::imread(image_name + ".jpg", cv::IMREAD_UNCHANGED);
	if (input_img.empty() == true) {
		// 画像データが読み込めなかったときは終了する
		std::cout << "Error : failed read img" << std::endl;
	}
	else {
		cv::Mat frame = kokubanCV::clickPointPerspectiveTransformation(input_img);
		a = kokubanCV::pulledOutChalkOnKokuban(frame);
		cv::imshow("binary", kokubanCV::binary(frame, 160));//画像を表示
		cv::waitKey(1);
	}
}

int main()
{
	//testChalk();
	Initialize();

	Update();
	std::string x;
	std::cin >> x;
	delete(eraserManager);
	cv::destroyAllWindows();
	return 0;
}

void Initialize()
{
	eraserManager = new Eraser::EraserManager();

	// ここをwebカメラからの映像にする
	inputImage = cv::imread("kokuban.jpg", cv::IMREAD_UNCHANGED);
}

void Update()
{

	while (true)
	{
		if (inputImage.empty() == true) {
			// 画像データが読み込めなかったときは終了する
			std::cout << "Error : failed read img" << std::endl;
			break;
		}
		else {
			cv::Mat frame = kokubanCV::binary(inputImage);// color_to_binary(input_img, 128);
			chalks = kokubanCV::pulledOutChalkOnKokuban(frame);
			//cv::imshow("binary", frame);//画像を表示
			inputImage = frame;
			//cv::waitKey(1);
		}
		eraserManager->chalkPoints = chalks;

		eraserManager->Update();
		eraserManager->DebugDraw(inputImage);
	}
}
