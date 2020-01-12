#include "kokubanCV.h"

namespace kokubanCV {

	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat binary_image)
	{
		//std::vector<std::vector<int>> chalkPoints(binary_image.cols, std::vector<int> (binary_image.rows));
		//黒板の範囲内を抽出
		//範囲内だけを走査するか範囲内を切り出してから全体を走査するのどっちが良いかな
		//cv::Mat kokubanImage = cv::Mat::regionOfImage();

		int width = binary_image.cols;
		int height = binary_image.rows;
		//std::vector<std::vector<int>>chalkPoints(width, std::vector<int>(height));
		std::vector<cv::Point> chalkPoints;

		cv::Vec3b zero = (0, 0, 0);

		cv::Mat_<cv::Vec3b> img(binary_image);

		for (int y = 0; y <height; ++y) {
			//y軸を走査
			cv::Vec3b* src = binary_image.ptr<cv::Vec3b>(y);
			//cv::Vec3b* p = &img(y, 0);
			//std::cout << "y = " <<  y <<" :  ";
			for (int x = 0; x < width; ++x) {
				//x軸を走査
				if (src[x][0] == 255 && src[x][1] == 255) {
					chalkPoints.push_back(cv::Point(x,y));
					std::cout << chalkPoints.back() << std::endl;
					//std::cout << src[x] << std::endl;
				}
				//std::cout << src[x];
				//std::cout << std::endl;
				//++p;
			}
		}
		std::cout << chalkPoints.size() <<   std::endl;
		// binary_image.forEach<uchar>([&chalkPoints](uchar& x, const int position[]) -> void {
		// 	if (x <= 0) {
		// 		chalkPoints.at(x.x).at(x.y) = 1;
		// 		std::cout << chalkPoints.at(position[0]).at(position[1]) << std::endl;
		// 	}
		// 	});

		//抽出した点群の座標を返す
		return chalkPoints;
	}

	cv::Mat binary(cv::Mat img)
	{
		int width = img.cols;
		int height = img.rows;
		cv::Mat binary_img = img;

		for (int y = 0; y < height -10; ++y) {
			//y軸を走査
			//cv::Vec3b* src = img.ptr<cv::Vec3b>(y);

			for (int x = 0; x < width; ++x) {
				//x軸を走査
				if (img.at<cv::Vec3b>(y, x)[0] > 128 && img.at<cv::Vec3b>(y, x)[1] > 128 && img.at<cv::Vec3b>(y, x)[2] > 128) {
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
