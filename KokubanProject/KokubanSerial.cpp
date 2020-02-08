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

bool KokubanSerial::checkRead()
{
	char head = read();
	if (head == receive_message)
	{
		return true;
	}
	return false;
}

cv::Point KokubanSerial::readMessage(cv::Point prev)
{
	cv::Point point;
	if ( receive_message == read() ) 
	{
		point.x = static_cast<int>(read());
		point.y = static_cast<int>(read());
	}

	fflush();

	// ‚È‚º‚©³‚µ‚¢’l‚Æˆê‚É(0,0)‚ª‘—‚ç‚ê‚Ä‚­‚é‚Ì‚Å‘ÎÇ—Ã–@“I‚É‘Î‰
	if (point == cv::Point(0, 0))
	{
		return prev;
	}
	else
	{
		return point;
	}
}

void KokubanSerial::stop()
{
	char message[2];
	message[0] = stop_message;
	message[1] = '\n';
	write(message, 2);
}

