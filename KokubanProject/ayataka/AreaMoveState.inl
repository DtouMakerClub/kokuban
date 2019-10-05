#include "AreaMoveState.h"

namespace Eraser
{
	AreaMoveState::AreaMoveState()
	{

	}

	AreaMoveState::~AreaMoveState()
	{

	}

	inline void AreaMoveState::Move(int index)
	{
		// 目的エリアの設定
		m_targetPos = AreaToPoint(index);
	}

	inline void AreaMoveState::CulcurateArea()
	{
		for (auto chalk : chalkPoints)
		{
			int area = PointToArea(chalk);
			m_areaWeight[area]++;
		}
	}

	inline int AreaMoveState::TargetAreaCheck()
	{
		int max = std::max_element(m_areaWeight[0], m_areaWeight[8]);

		return max;
	}

	void AreaMoveState::Update()
	{
		// エリアの重み算出
		CulcurateArea();

		// 移動するためのエリアを選出
		m_targetIndex = TargetAreaCheck();

		// 移動するための座標計算
		Move(m_targetIndex);

		// 目標座標の送信

	}


}