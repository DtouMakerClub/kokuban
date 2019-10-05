#pragma once

#include "EraserStateBase.h"

namespace Eraser 
{
	class AreaMoveState : public EraserStateBase
	{
	public:
		AreaMoveState();
		~AreaMoveState();

		void Update();
		void Move(int index);
	private:
		// エリアの重みを計算
		void CulcurateArea();
		// エリアの番号から座標を取得
		cv::Point2i AreaToPoint(int index);
		// 移動するエリアの決定
		int TargetAreaCheck();

		int m_areaWeight[9];				// 各エリアの重み
		int m_targetIndex;					// 目標位置のエリア
		cv::Point2i m_targetPos;			// 目標位置の座標
	};
}
