#include "SerialWrapper.h"

#include <tchar.h>

SerialWrapper::SerialWrapper(int comNum)
{
	TCHAR com[10];
	_stprintf_s(com, 10, _T("COM%d"), comNum);

	// シリアルポートを開く
	com_port = CreateFile(com, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
}

// 開いていたシリアルデバイスを破棄する
 SerialWrapper::~SerialWrapper()
{
	// シリアルポートを閉じる
	CloseHandle(com_port);
}

void SerialWrapper::begin(unsigned long speed)
{
	DCB dcb; // シリアルポートの構成情報が入る構造体
	GetCommState(com_port, &dcb); // 現在の設定値を読み込み

	dcb.BaudRate = speed; // 速度
	dcb.ByteSize = 8; // データ長
	dcb.Parity = NOPARITY; // パリティ
	dcb.StopBits = ONESTOPBIT; // ストップビット長
	dcb.fOutxCtsFlow = FALSE; // 送信時CTSフロー
	dcb.fRtsControl = RTS_CONTROL_ENABLE; // RTSフロー

	// 変更した設定値を書き込み
	SetCommState(com_port, &dcb);
}

size_t SerialWrapper::write(char byte)
{
	char* sentData = &byte; // 送信する1byte
	DWORD numberOfPut; // 実際に送信したbyte数

	// ポートへ送信
	WriteFile(com_port, sentData, 1, &numberOfPut, NULL);

	return numberOfPut;
}

size_t SerialWrapper::write(const char buf[], int len)
{
	DWORD lengthOfSent = len; // 送信するbyte数
	DWORD numberOfPut; // 実際に送信したbyte数

	// ポートへ送信
	WriteFile(com_port, buf, lengthOfSent, &numberOfPut, NULL);

	return numberOfPut;
}

int SerialWrapper::available()
{
	//受信データ数を調べる
	DWORD errors;
	COMSTAT comStat;
	ClearCommError(com_port, &errors, &comStat);

	int lengthOfRecieved = comStat.cbInQue; // 受信したメッセージ長を取得する

	return lengthOfRecieved;
}

unsigned char SerialWrapper::read()
{
	//受信データがない場合は読み込まない
	if (available() < 1)
	{
		return 0;
	}

	unsigned char buf[1]; // 受信データ格納用
	DWORD numberOfRead; // 実際に受信したバイト数

	//データ受信
	bool result = ReadFile(com_port, buf, 1, &numberOfRead, NULL);

	if (result == FALSE)
	{
		return 0;
	}
	else
	{
		return buf[0];
	}
}

void SerialWrapper::fflush()
{
	unsigned char buf[1]; // 受信データ格納用
	DWORD numberOfRead; // 実際に受信したバイト数
	
	while(available()){
		ReadFile(com_port, buf, 1, &numberOfRead, NULL);
	}
}