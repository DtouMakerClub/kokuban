#include "EraserStateBase.h"

namespace Eraser
{
	EraserStateBase::EraserStateBase()
	{
		// テスト用チョーク点群のセット
		TestChalk();
	}

	EraserStateBase::~EraserStateBase()
	{

	}

	void EraserStateBase::Update()
	{
		// 検出したチョークの取得


		// 状態の更新判定
		UpdateState();
	}

	inline void EraserStateBase::TestChalk()
	{
		for (int i = 0; i < 10; i++)
		{
			cv::Point2i point(i + 50, i + 50);

			chalkPoints.push_back(point);
		}
	}

	inline void EraserStateBase::UpdateState()
	{
		// 現エリアの重みが小さくなったら(1/10を想定)
		// 1024 / 3 = 340 / 10 = だいたい30ピクセルぐらい
		if (m_weight < 30)
		{
			m_status = AreaMove;
		}

		// エリアの移動が完了したら
		if (m_nowAreaIndex == m_targetIndex)
		{
			m_status = PointMove;
		}

		switch (m_status)
		{
		case EraserStatus::AreaMove:
			break;
		case EraserStatus::PointMove:
			break;
		default:
			break;
		}
	}

	inline void EraserStateBase::CulcurateArea()
	{
		for (auto chalk : chalkPoints)
		{
			int area = PointToArea(chalk);
			m_areaWeight[area]++;
		}
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
			std::cout << "[ERASER]x座標上は黒板にいません。" << std::endl;
		}

		// 行の判定
		// ピクセルサイズの1024(変数や定数化するのがスマート)
		if (0 < pos.y && pos.y < 1024 * (1/3) )
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
			std::cout << "[ERASER]y座標上は黒板にいません。" << std::endl;
		}

		// 現状だと(3×3なので)
		// [0][3][6]
		// [1][4][7]
		// [2][5][8]
		return (x_index * 3) + y_index;
	}
	
}