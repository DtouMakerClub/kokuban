#pragma once
#include <opencv2/opencv.hpp>

namespace kokubanCV {

	/// <summary>
	/// 黒板上のチョークで書かれた位置を特定
	/// 黒板の外形の座標を貰い黒板の範囲内にある2値化した際に色が残った場所の座標を返す
	/// </summary>
	/// <param name="binary_img">2値化した画像</param>
	/// <param name=" threshold">2値化時の閾値</param>
	/// <returns></returns>
	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat image, int threshold = 128);

/// <summary>
/// 自作2値か関数
/// 入力画像がjpgならまともに動く
/// pngは変になる（たぶん配列に入っている順番とか要素が違う）
/// </summary>
/// <param name="img"></param>
/// <param name=" threshold">2値化時の閾値</param>
/// <returns></returns>
	cv::Mat binary(cv::Mat img, int threshold = 128);
	
	cv::VideoCapture openVideoFile(std::string video_name);
	template <class Fn> void funcVideo(std::string video_name, std::string save_file_name, Fn fn);

}