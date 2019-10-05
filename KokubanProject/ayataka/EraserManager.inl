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
		// 
		m_state.Update();

		// 状態の更新
		UpdateState();

		// 目的地の送信

	}

	inline int EraserManager::CheckAreaIndex(cv::Point2i now_pos)
	{
		int x_index = 0;
		int y_index = 0;

		// 行の判定
		// ピクセルサイズの1024(変数や定数化するのがスマート)
		if (0 < now_pos.x && now_pos.x < 1024/widthSection)
		{
			x_index = 0;
		}
		else if (1024 * widthSection < now_pos.x && now_pos.x < 1024 * 2/widthSection)
		{
			x_index = 1;
		}
		else if (1024 * 2 / widthSection < now_pos.x && now_pos.x < 1024)
		{
			x_index = 2;
		}
		else
		{
			std::cout << "[ERASER]x座標上は黒板にいません。";
		}

		// 行の判定
		// ピクセルサイズの1024(変数や定数化するのがスマート)
		if (0 < now_pos.y && now_pos.y < 1024 / heightSection)
		{
			y_index = 0;
		}
		else if (1024 * heightSection < now_pos.y && now_pos.y < 1024 * 2 / heightSection)
		{
			y_index = 1;
		}
		else if (1024 * 2 / heightSection && now_pos.y < 1024)
		{
			y_index = 2;
		}
		else
		{
			std::cout << "[ERASER]y座標上は黒板にいません。";
		}

		// 現状だと(3×3なので)
		// [0][3][6]
		// [1][4][7]
		// [2][5][8]
		return (x_index * 3) + y_index;
	}

	inline void EraserManager::UpdateState()
	{
		// 現エリアの重みが小さくなったら
		
		
		// エリアの移動が完了したら
		

	}
}