#include "AreaMoveState.h"

namespace Eraser
{
	AreaMoveState::AreaMoveState()
	{

	}

	AreaMoveState::~AreaMoveState()
	{

	}

	void AreaMoveState::Update()
	{
		// 移動するためのエリアを選出
		m_targetIndex = TargetAreaCheck();

		// 移動するための座標計算
		Move(m_targetIndex);

		EraserStateBase::Update();
	}

	inline void AreaMoveState::Move(int index)
	{
		// 目的エリアの設定
		m_targetPoint = AreaToPoint(index);
	}

	inline int AreaMoveState::TargetAreaCheck()
	{
		int max = std::max_element(m_areaWeight[0], m_areaWeight[8]);

		return max;
	}
}