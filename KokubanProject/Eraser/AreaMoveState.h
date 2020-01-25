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
		// 移動するエリアの決定
		int TargetAreaCheck();

	};
}
