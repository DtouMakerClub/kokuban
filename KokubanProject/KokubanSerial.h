#pragma once
#include <opencv2/opencv.hpp>
#include "SerialWrapper.h"

class KokubanSerial : SerialWrapper
{
	int baud_rate = 9600;
	char start_message = 'I';
	char stop_message = 'F';
	char move_message = 'M';
	char receive_message = 'K';
public:
	KokubanSerial(int com_num);
	virtual ~KokubanSerial();

	void start();
	void sendMessage(uchar x, uchar y);
	bool checkRead();
	cv::Point readMessage(cv::Point prev);
	void stop();

};
