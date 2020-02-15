#include "kokubanCV.h"

namespace kokubanCV {

	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat image, int threshold)
	{
		int width = image.cols;
		int height = image.rows;
		std::vector<cv::Point> chalkPoints;
		cv::Vec3b zero = (0, 0, 0);
		cv::Mat_<cv::Vec3b> img(image);

		for (int y = 0; y <height; ++y) {
			//y軸を走査
			cv::Vec3b* src = image.ptr<cv::Vec3b>(y);
			for (int x = 0; x < width; ++x) {
				//x軸を走査
				if (img.at<cv::Vec3b>(y, x)[0] * 0.299 + img.at<cv::Vec3b>(y, x)[1] * 0.587 + img.at<cv::Vec3b>(y, x)[2] * 0.114 > threshold) {
					chalkPoints.push_back(cv::Point(x,y));
					//std::cout << chalkPoints.back() << std::endl;
				}
			}
		}
		//std::cout << chalkPoints.size() <<   std::endl;

		//抽出した点群の座標を返す
		return chalkPoints;
	}


	cv::Mat binary(cv::Mat img, int threshold)
	{
		int width = img.cols;
		int height = img.rows;
		cv::Mat binary_img = img;

		for (int y = 0; y < height -10; ++y) {
			//y軸を走査
			//cv::Vec3b* src = img.ptr<cv::Vec3b>(y);

			for (int x = 0; x < width; ++x) {
				//x軸を走査
				if (img.at<cv::Vec3b>(y, x)[0] * 0.299 +  img.at<cv::Vec3b>(y, x)[1] * 0.587 +  img.at<cv::Vec3b>(y, x)[2]  * 0.114 > threshold) {
					binary_img.at<cv::Vec3b>(y, x)[0] = 255;
					binary_img.at<cv::Vec3b>(y, x)[1] = 255;
					binary_img.at<cv::Vec3b>(y, x)[2] = 255;
				}else {
					binary_img.at<cv::Vec3b>(y, x)[0] = 0;
					binary_img.at<cv::Vec3b>(y, x)[1] = 0;
					binary_img.at<cv::Vec3b>(y, x)[2] = 0;
				}
			}
		}

		//抽出した点群の座標を返す
		return binary_img;
	}

	cv::Mat clickPointPerspectiveTransformation(cv::Mat input_image, cv::Point2f click_point[4])
	{
		kokubanCV::mouseParam mouse;
		cv::Mat frame = input_image;
		//cv::Size size = { int(input_image.cols * 0.5), int(input_image.rows * 0.5)};
		//cv::resize(input_image, frame, size);
		int width = frame.cols;
		int height = frame.rows;

		const cv::Point2f dst_pt[4] = {
					cv::Point2f(0,						  0),
					cv::Point2f(0,						  0 + height - 1),
					cv::Point2f(0 + width ,       0),
					cv::Point2f(0 + width ,       0 + height ) };

		int i = 0;
		cv::Mat dst = cv::Mat::zeros(height, width, CV_8UC3);

		cv::Mat homography_matrix = getPerspectiveTransform(click_point, dst_pt);
		warpPerspective(frame, dst, homography_matrix, dst.size());
		return dst;
	}

	void returnClickPoints(cv::Mat input_image, cv::Point2f point[4])
	{
		kokubanCV::mouseParam mouse;
		cv::Mat frame = input_image;
		cv::imshow("input_image", frame);//画像を表示
		cv::setMouseCallback("input_image", kokubanCV::mouseCallback, &mouse);

		int i = 0;

		std::cout << "click point 4 times" << std::endl;
		while (1) {
			cv::waitKey(2);
			//左クリックした座標を取得
			if (mouse.event == cv::EVENT_LBUTTONDOWN && i <= 4) {
				//クリック後のマウスの座標を出力
				std::cout << mouse.x << " , " << mouse.y << std::endl;
				// 左上、左下、右上、右下の順で入力
				point[i] = cv::Point2f(mouse.x, mouse.y);
				++i;
				cv::waitKey(200);
			}
			//4点クリックで終了
			else if( i >= 4){
				cv::destroyAllWindows();
				break;
			}
		}
	}




	void mouseCallback(int event, int x, int y, int flags, void* userdata)
	{
		mouseParam* ptr = static_cast<mouseParam*> (userdata);

		ptr->x = x;
		ptr->y = y;
		ptr->event = event;
		ptr->flags = flags;
	}

	cv::Mat maskOrange(cv::Mat input)
	{
		cv::Mat result;

		const unsigned char h = 10;
		const unsigned char s = 143;
		const unsigned char v = 121;
		auto low = cv::Scalar(0, s - 20, v - 20);
		auto high = cv::Scalar(h/2 + 30, 255, 255);
		
		cv::Mat hsv_input;
		cv::cvtColor(input, hsv_input, cv::COLOR_BGR2HSV);

		cv::Mat mask;
		cv::inRange(hsv_input, low, high, mask);

		cv::Mat dilateElement(5, 5, CV_8U, cv::Scalar(1));
		cv::dilate(mask, mask, dilateElement);
		cv::dilate(mask, mask, dilateElement);
		cv::dilate(mask, mask, dilateElement);
		
		//cv::dilate(mask, mask, dilateElement);
		//cv::dilate(mask, mask, dilateElement);
		//cv::dilate(mask, mask, dilateElement);

		cv::bitwise_not(mask, mask);
		cv::bitwise_and(input, input, result, mask);
		cv::imshow("mask", mask);

		cv::waitKey(1);

		return result;
	}


	cv::VideoCapture openVideoFile(std::string video_name)
	{
		cv::VideoCapture cap;
		cap.open(video_name);

		if (cap.isOpened() == false) {
			std::cout << video_name << "  cant open " << std::endl;
			return -1;
		}
		return cap;
	}

	template <class Fn> void funcVideo(std::string video_name, std::string save_file_name, Fn fn)
	{
		cv::VideoCapture cap = openVideoFile(video_name);
		// 作成する動画ファイルの諸設定
		int    fourcc, width, height;
		double fps;

		width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);	// フレーム横幅を取得
		height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);	// フレーム縦幅を取得
		fps = cap.get(cv::CAP_PROP_FPS);					// フレームレートを取得
		fourcc = cv::VideoWriter::fourcc('M', 'P', '4', 'V');	// エンコード形式を指定
		// * エンコード形式 "XVID" = AVI, "MP4V" = MPEG4, "WMV1" = WMV

		// 動画ファイルを書き出すためのオブジェクトを宣言する
		cv::VideoWriter writer;
		writer.open(save_file_name, fourcc, fps, cv::Size(width, height));

		cv::Mat frame, dst;

		while (true) {
			
			cap >> frame;

			if (frame.empty() == true) {
				break;
			}

			//何か処理をする
			dst = fn(frame);

			cv::imshow("変換中", frame);

			writer << dst;
			cv::waitKey(1);
		}

		writer.release();
		std::cout << "end " << std::endl;
	}
}
