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
	char message[4];
	message[0] = 'M';
	message[1] = x;
	message[2] = y;
	message[3] = '\n';

	write(message, 4);
}

cv::Point KokubanSerial::readMessage()
{
	cv::Point point;
	if ( move_message == read()) {
		point.x = read();
		point.y = read();
	}
	return point;
}

void KokubanSerial::stop()
{
	char message[2];
	message[0] = stop_message;
	message[1] = '\n';
	write(message, 2);
}

