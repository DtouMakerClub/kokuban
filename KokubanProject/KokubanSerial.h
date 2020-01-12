#pragma once
#include <opencv2/opencv.hpp>

class KokubanSerial : SerialWrapper
{
	int baud_rate = 9600;
	char start_message = 'I';
	char stop_message = 'F';
	char move_message = 'M';
public:
	KokubanSerial(int com_num);
	virtual ~KokubanSerial();

	void start();
	void sendMessage(uchar x, uchar y);
	cv::Point readMessage();
	void stop();

};
