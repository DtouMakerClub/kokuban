#include "SerialWrapper.h"
#include "KokubanSerial.h"

KokubanSerial::KokubanSerial(int com_num) : SerialWrapper(com_num)
{
	begin(baud_rate);
}

KokubanSerial::~KokubanSerial()
{
}

void KokubanSerial::start()
{
	char message[2];
	message[0] = start_message;
	message[1] = '\n';
	write(message, 2);
}

void KokubanSerial::sendMessage(uchar x, uchar y)
{
	if (x < 0)		x = 0;
	if (y < 0)		y = 0;
	if (x > 255)	x = 255;
	if (y > 255)	y = 255;

	char message[4];
	message[0] = 'M';
	message[1] = x;
	message[2] = y;
	message[3] = '\n';

	write(message, 4);
}

bool KokubanSerial::checkRead()
{
	char head = read();
	if (head == receive_message)
	{
		return true;
	}
	return false;
}

bool KokubanSerial::isReadableMessage()
{
	//受信データがない場合は読み込まない
	if (available() < 1)
	{
		return false;
	}

	return true;
}

cv::Point KokubanSerial::readMessage(cv::Point prev)
{
	cv::Point point;
	if ( receive_message == read() ) 
	{
		point.x = read();
		point.y = read();
	}

	fflush();

	return point;
}

void KokubanSerial::stop()
{
	char message[2];
	message[0] = stop_message;
	message[1] = '\n';
	write(message, 2);
}

