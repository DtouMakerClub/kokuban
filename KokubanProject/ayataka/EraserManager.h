#pragma once

#include <opencv2\opencv.hpp>

namespace Eraser 
{
	enum EraserStatus
	{
		AreaMove = 0,
		PointMove = 1,
	};

	class EraserManager
	{
	public:
		EraserManager();
		~EraserManager();

		int const heightSection = 3;
		int const widthSection = 3;

	private:
		void Update();
		//どこにいるかを判定
		int CheckAreaIndex(cv::Point2i now_pos);
		// 状態の更新
		void UpdateState();

		//いるエリアの近傍を取得
		//近傍エリアの重みを計算
		
		// 現在位置(x,y)
		cv::Point2i m_eraserPos;
		// 現在位置のエリアインデックス
		int m_nowArea;
		// 現在いるエリアの重み
		int m_weight;
		// 状態管理
		EraserStateBase m_state;
	};
}
