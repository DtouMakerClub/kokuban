#include "kokubanCV.h"

namespace kokubanCV {

	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat image, int threshold, int ksize)
	{
		int width = image.cols;
		int height = image.rows;
		std::vector<cv::Point> chalkPoints;
		cv::Vec3b zero = (0, 0, 0);
		cv::Mat_<cv::Vec3b> img(image);
	    assert(ksize % 2 == 1);
		cv::medianBlur(image, img, ksize);

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


	cv::Mat binary(cv::Mat img, int threshold, int ksize)
	{
		int width = img.cols;
		int height = img.rows;
		cv::Mat binary_img = img;
		assert(ksize % 2 == 1);
		cv::medianBlur(img, binary_img,  ksize);

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

	cv::Mat color_to_binary(cv::Mat frame, int threshold, bool is_bright) {
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

	std::vector<std::vector<cv::Point>> conto(cv::Mat img) {
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

			cv::imshow("win", fn(frame, 128, true));//画像を表示．
			const int key = cv::waitKey(1);
			if (key == 'q') //qボタンが押されたとき
			{
				break;
			}
		}

		cap.release();
		cv::destroyAllWindows();
	}

}
