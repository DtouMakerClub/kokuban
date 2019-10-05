#include "EraserStateBase.h"

namespace Eraser
{
	EraserStateBase::EraserStateBase()
	{

	}

	EraserStateBase::~EraserStateBase()
	{

	}

	void EraserStateBase::Update()
	{

	}

	inline cv::Point2i EraserStateBase::AreaToPoint(int index)
	{
		cv::Point2i result;

		// ピクセルサイズを1024としてその比率で座標を算出
		if (index < 3)
		{
			result.x = 1024 * 1 / 6;
		}
		else if (index < 6)
		{
			result.x = 1024 * 1 / 2;
		}
		else if (index < 9)
		{
			result.x = 1024 * 5 / 6;
		}

		if (index % 3 == 0)
		{
			result.y = 1024 * 1 / 6;
		}
		else if (index % 3 == 1)
		{
			result.y = 1024 * 1 / 2;
		}
		else if (index % 3 == 2)
		{
			result.y = 1024 * 5 / 6;
		}

		return result;
	}

	inline int EraserStateBase::PointToArea(cv::Point2i pos)
	{
		int x_index = 0;
		int y_index = 0;

		// 行の判定
		// ピクセルサイズの1024(変数や定数化するのがスマート)
		if (0 < pos.x && pos.x < 1024 * (1/3) )
		{
			x_index = 0;
		}
		else if (1024 * (1/3) < pos.x && pos.x < 1024 * (2/3) )
		{
			x_index = 1;
		}
		else if (1024 * (2/3) < pos.x && pos.x < 1024)
		{
			x_index = 2;
		}
		else
		{
			std::cout << "[ERASER]x座標上は黒板にいません。";
		}

		// 行の判定
		// ピクセルサイズの1024(変数や定数化するのがスマート)
		if (0 < pos.y && pos.y < 1024 / (1/3) )
		{
			y_index = 0;
		}
		else if (1024 * (1/3) < pos.y && pos.y < 1024 * (2/3))
		{
			y_index = 1;
		}
		else if (1024 * (2/3) && pos.y < 1024)
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
	
}