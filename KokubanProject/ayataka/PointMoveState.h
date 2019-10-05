#pragma once

#include <opencv2\opencv.hpp>

#include "EraserStateBase.h"

namespace Eraser 
{
	class PointMoveState : public EraserStateBase
	{
	public:
		PointMoveState();
		~PointMoveState();

		void Update();
		void Move();
	private:
		// 近傍のエリアの重みを計算

		// 自身の位置に最近傍の点を取得する
		cv::Point2i FindNearest(int index);

		int m_nowArea;						// 自分が現在いるエリアインデックス
		cv::Point2i m_targetPos;			// 目標位置の座標
	};
}
