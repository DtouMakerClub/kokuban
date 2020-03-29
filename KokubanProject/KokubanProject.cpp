#include <opencv2/opencv.hpp>
#include "kokubanCV.h"

#include "KokubanProject.h"

#include "Eraser/EraserManager.inl"



void test_binary() {
	cv::Mat input_img = cv::imread("sample.png", cv::IMREAD_UNCHANGED);
	if (input_img.empty() == true) {
		// 画像データが読み込めなかったときは終了する
		std::cout << "Error : failed read img" << std::endl;
	}
	else {
		cv::Mat frame = kokubanCV::color_to_binary(input_img, 18);
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
	cv::Point2f src_pt[4] = {};
	bool flag = true;


	while (true) {

		cap >> frame;

		if (frame.empty() == true) {
			break;
		}

		if (flag) {
			kokubanCV::returnClickPoints(frame, src_pt);
			cv::waitKey(2000);
			flag = false;
		}

		//何か処理をする
		dst = kokubanCV::clickPointPerspectiveTransformation(frame, src_pt);

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


void  test_Mouse() {
	std::string image_name = "kokuban";
	cv::Mat input_img = cv::imread(image_name + ".jpg", cv::IMREAD_UNCHANGED);
	if (input_img.empty() == true) {
		// 画像データが読み込めなかったときは終了する
		std::cout << "Error : failed read img" << std::endl;
	}
	else {
		//cv::Mat frame = kokubanCV::clickPointPerspectiveTransformation(input_img);
		//a = kokubanCV::pulledOutChalkOnKokuban(frame);
		cv::Mat binary = kokubanCV::binary(input_img, 160);
		cv::imshow("binary", binary);//画像を表示
		//cv::imwrite("Median.jpg", binary);
		cv::waitKey(1);
	}
}

int main()
{
	//test_Mouse();
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
	// ここをwebカメラからの映像にする
	cap.open(0);
	if (!cap.isOpened()) {
		std::cout << "Error : failed open camera" << std::endl;
		//break;
	}
	cap.read(inputImage);

	kokubanCV::returnClickPoints(inputImage, src_pt);
	cv::waitKey(2000);
	eraserManager = new Eraser::EraserManager();
}

void Update()
{
	cv::Mat frame;
	//cv::Mat dst;

	while (true)
	{
		if (inputImage.empty() == true) 
		{
			// 画像データが読み込めなかったときは終了する
			std::cout << "Error : failed read img" << std::endl;
			break;
		}
		else 
		{
			cap.read(inputImage);
			frame = kokubanCV::clickPointPerspectiveTransformation(inputImage, src_pt);// color_to_binary(input_img, 128);
			frame = kokubanCV::maskOrange(frame);
			frame = kokubanCV::binary(frame, 180);
			eraserManager->chalkPoints = kokubanCV::pulledOutChalkOnKokuban(frame, 180);
			cv::imshow("camera", inputImage);//画像を表示
			//inputImage = frame;
			int key = cv::waitKey(1);
			
			// Q key 「終了処理」
			if (key == 113)
			{
				break;
			}
		}

		eraserManager->Update();
		eraserManager->InputKey();
		eraserManager->DebugDraw(frame);
	}
}
