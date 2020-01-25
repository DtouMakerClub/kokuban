#include "EraserManager.h"


namespace Eraser
{
	EraserManager::EraserManager()
	{
		m_state = EraserMoveState::READY;
	}

	EraserManager::~EraserManager()
	{

	}

	void EraserManager::Update()
	{
		// 現在位置の受信
		//m_eraserPos = ReceivePosition();

		// 目標位置の更新処理
		UpdateMove();

		// 各状態の更新処理
		UpdateState();

		// デバッグ用

		// 目的地の送信
		if (SendTargetPosition() == true)
		{
			std::cout << "送信に成功" << std::endl;
		}
		else
		{
			std::cout << "送信に失敗しました。" << std::endl;
		}

	}

	void EraserManager::UpdateMove()
	{
		switch (m_state)
		{
		case EraserMoveState::READY:
			// ここでInit処理を行うつもり
			m_state = EraserMoveState::AREA;
			break;
		case EraserMoveState::AREA:
			// 移動するためのエリアを選出
			m_targetIndex = TargetAreaCheck();
			m_targetPoint = AreaToPoint(m_targetIndex);
			break;
		case EraserMoveState::POINT:
			CulcurateArea();
			// 自身から最も近い点を取得する
			m_targetPoint = FindNearest(m_nowAreaIndex);
			break;
		default:
			break;
		}
	}

	inline cv::Point2i EraserManager::FindNearest(int index)
	{
		cv::Point2i target;
		float minDist = 100000.0f;
		float dist = 0.0f;

		for (auto chalk : chalkPoints)
		{
			dist = cv::norm(chalk - m_nowPoint);
			if (minDist > dist)
			{
				target = chalk;
				minDist = dist;
			}
		}

		return target;
	}
	inline void EraserManager::UpdateState()
	{
		// 現エリアの重みが小さくなったら(1/10を想定)
		// 1024 / 3 = 340 / 10 = だいたい30ピクセルぐらい
		if (m_nowAreaWeight < 30)
		{
			m_state = EraserMoveState::AREA;
		}

		// エリアの移動が完了したら
		if (m_nowAreaIndex == m_targetIndex)
		{
			m_state = EraserMoveState::POINT;
		}
	}

	cv::Point2i EraserManager::ReceivePosition()
	{
		cv::Point2i postion;

		return postion;
	}

	inline bool EraserManager::SendTargetPosition()
	{
		// 送信するときに自分の位置に近すぎる座標は省く

		// 送信に失敗したとき
		return true;
	}

	inline int EraserManager::TargetAreaCheck()
	{
		int max = std::max_element(m_areaWeight[0], m_areaWeight[8]);

		return max;
	}
	inline cv::Point2i EraserManager::AreaToPoint(int index)
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

	inline int EraserManager::PointToArea(cv::Point2i pos)
	{
		int x_index = 0;
		int y_index = 0;

		// 行の判定
		// ピクセルサイズの1024(変数や定数化するのがスマート)
		if (0 < pos.x && pos.x < 1024 * (1 / 3))
		{
			x_index = 0;
		}
		else if (1024 * (1 / 3) < pos.x && pos.x < 1024 * (2 / 3))
		{
			x_index = 1;
		}
		else if (1024 * (2 / 3) < pos.x && pos.x < 1024)
		{
			x_index = 2;
		}
		else
		{
			std::cout << "[ERASER]x座標上は黒板にいません。" << std::endl;
		}

		// 行の判定
		// ピクセルサイズの1024(変数や定数化するのがスマート)
		if (0 < pos.y && pos.y < 1024 * (1 / 3))
		{
			y_index = 0;
		}
		else if (1024 * (1 / 3) < pos.y && pos.y < 1024 * (2 / 3))
		{
			y_index = 1;
		}
		else if (1024 * (2 / 3) && pos.y < 1024)
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