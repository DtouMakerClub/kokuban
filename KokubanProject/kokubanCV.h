#pragma once
#include <opencv2/opencv.hpp>

namespace kokubanCV {

	/// <summary>
	/// ����̃`���[�N�ŏ����ꂽ�ʒu�����
	/// ���̊O�`�̍��W��Ⴂ���͈͓̔��ɂ���2�l�������ۂɐF���c�����ꏊ�̍��W��Ԃ�
	/// </summary>
	/// <param name="binary_img">2�l�������摜</param>
	/// <param name="kokuban_contours">���̊O�`�̒��_���W</param>
	/// <returns></returns>
	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat binary_image);
	cv::Mat binary(cv::Mat img);
	
	cv::VideoCapture openVideoFile(std::string video_name);
	template <class Fn> void funcVideo(std::string video_name, std::string save_file_name, Fn fn);

}