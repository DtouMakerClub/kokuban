#pragma once

#include <opencv2\opencv.hpp>

namespace Eraser 
{
	class EraserManager
	{
	public:
		EraserManager();
		~EraserManager();

		int const heightSection = 3;
		int const widthSection = 3;

	private:
		void Update();

		// 現在位置の受信(xyステージとの)
		cv::Point2i ReceivePosition();
		// 目標位置の送信
		bool SendTargetPosition();
		// 現在位置(x,y)
		cv::Point2i m_eraserPos;
		// 現在位置のエリアインデックス
		int m_nowArea;

		// 状態管理
		EraserStateBase m_state;
	};
}
