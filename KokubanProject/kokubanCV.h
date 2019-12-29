#pragma once
#include <opencv2/opencv.hpp>

namespace kokubanCV {

	/// <summary>
	/// 黒板上のチョークで書かれた位置を特定
	/// 黒板の外形の座標を貰い黒板の範囲内にある2値化した際に色が残った場所の座標を返す
	/// </summary>
	/// <param name="binary_img">2値化した画像</param>
	/// <param name="kokuban_contours">黒板の外形の頂点座標</param>
	/// <returns></returns>
	std::vector<std::vector<int>> pulledOutChalkOnKokuban(cv::Mat binary_image, std::vector<std::vector<cv::Point>> kokuban_contours);
	
	cv::VideoCapture openVideoFile(std::string video_name);
	template <class Fn> void funcVideo(std::string video_name, std::string save_file_name, Fn fn);

}