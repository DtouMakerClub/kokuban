#include "EraserManager.h"

#include "EraserStateBase.inl"


namespace Eraser
{
	EraserManager::EraserManager()
	{

	}

	EraserManager::~EraserManager()
	{

	}

	void EraserManager::Update()
	{
		// 現在位置の受信
		m_eraserPos = ReceivePosition();

		// 各状態の更新処理
		m_state.Update();

		// 目的地の送信
		if (SendTargetPosition() == true)
		{

		}
		else
		{
			printf("送信に失敗しました。");
		}

	}

	cv::Point2i EraserManager::ReceivePosition()
	{
	}

	inline bool EraserManager::SendTargetPosition()
	{
		// 送信に失敗したとき
		return false;
	}

}