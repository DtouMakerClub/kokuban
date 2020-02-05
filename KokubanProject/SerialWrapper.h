#ifndef __SerialWrapper__
#define __SerialWrapper__

#include <Windows.h>

//http://www.nyankobass.net/archives/133
class SerialWrapper
{
	HANDLE com_port;

public:
	// arg1 : COMポートの番号を指定
	// 指定したシリアルデバイスを開く
	SerialWrapper(int comNum);

	// 開いていたシリアルデバイスを破棄する
	virtual ~SerialWrapper();

	// arg1 : 伝送速度[bps]
	// シリアル通信の初期化
	void begin(unsigned long speed);

	// arg1 : 送信する1byte
	// 1byteのデータを送信する
	// 実際に送信したbyte数を返す
	virtual size_t write(char byte);

	// arg1 : 送信するbyte列
	// arg2 : 送信するbyte数
	// 複数byteのデータを一度に送信する
	// 実際に送信したbyte数を返す
	virtual size_t write(const char buf[], int len);

	// 受信バッファに溜まったbyte数を返す
	virtual int available();

	// 受信バッファから1byte読み込む
	// 失敗した場合-1を返す
	virtual unsigned char read();

	// 受信バッファをクリアする
	virtual void fflush();
};

#endif __SerialWrapper__