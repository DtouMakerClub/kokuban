#include "EraserManager.h"


namespace Eraser
{
	EraserManager::EraserManager()
	{
		m_state = EraserMoveState::READY;
		
		m_eraserPos = cv::Point2i(20, 20);
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
			//std::cout << "送信に成功" << std::endl;
		}
		else
		{
			std::cout << "送信に失敗しました。" << std::endl;
		}

		m_nowAreaIndex = PointToArea(m_eraserPos);
		cv::Point2i vec = m_targetPoint - m_eraserPos;
		vec /= cv::norm(vec);
		m_eraserPos += vec * 2;
	}

	void EraserManager::UpdateMove()
	{
		switch (m_state)
		{
		case EraserMoveState::READY:
			// ここでInit処理を行うつもり
			m_state = EraserMoveState::AREA;
			CulcurateArea();
			break;
		case EraserMoveState::AREA:
			// 移動するためのエリアを選出
			m_targetIndex = TargetAreaCheck();
			m_targetPoint = AreaToPoint(m_targetIndex);
			break;
		case EraserMoveState::POINT:
			// 自身から最も近い点を取得する
			m_targetPoint = FindNearest(m_nowAreaIndex);
			break;
		default:
			break;
		}
	}

	inline void EraserManager::DebugDraw(cv::Mat img)
	{
		//img = cv::Scalar(255, 255, 255);

		// チョークリーナー
		cv::Point p1(20, 80);
		cv::rectangle(img, m_eraserPos, m_eraserPos + p1, cv::Scalar(255, 0, 0), 10, cv::LINE_4);

		// チョーク
		//for (auto chalk : chalks)
		//{
		//	cv::rectangle(img, chalk, chalk + p1, cv::Scalar(0, 0, 0), 1, cv::LINE_4);
		//}


		std::cout << "pos:" << m_eraserPos << std::endl;
		std::cout << "area:" << m_nowAreaIndex << std::endl;
		std::cout << "target:" << m_targetIndex << std::endl;

		cv::imshow("test", img);
		cv::waitKey(1);
	
	}

	inline cv::Point2i EraserManager::FindNearest(int index)
	{
		cv::Point2i target;
		float minDist = 100000.0f;
		float dist = 0.0f;

		for (auto chalk : chalkPoints)
		{
			int area = PointToArea(chalk);
			if (area == m_nowAreaIndex)
			{
				dist = static_cast<float>(cv::norm(chalk - m_eraserPos));
				if (minDist > dist)
				{
					target = chalk;
					minDist = dist;
				}
			}
		}

		return target;
	}
	inline void EraserManager::UpdateState()
	{
		// 現エリアの重みが小さくなったら(1/10を想定)
		if (m_areaWeight[m_nowAreaIndex] < ((CAMERA_RESOLUTION.x * CAMERA_RESOLUTION.y) / 3) / 10 )
		{
			m_state = EraserMoveState::AREA;
		}

		// エリアの移動が完了したら
		else if (m_nowAreaIndex == m_targetIndex)
		{
			m_state = EraserMoveState::POINT;
			CulcurateArea();
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
		int max = 0;
		int index = 0;

		for (int i = 0; i < 9; ++i)
		{
			if (max < m_areaWeight[i])
			{
				max = m_areaWeight[i];
				index = i;
			}
		}

		return index;
	}
	inline cv::Point2i EraserManager::AreaToPoint(int index)
	{
		cv::Point2i result;

		if (index < 3)
		{
			result.x = CAMERA_RESOLUTION.x * 1 / 6;
		}
		else if (index < 6)
		{
			result.x = CAMERA_RESOLUTION.x * 1 / 2;
		}
		else if (index < 9)
		{
			result.x = CAMERA_RESOLUTION.x * 5 / 6;
		}

		if (index % 3 == 0)
		{
			result.y = CAMERA_RESOLUTION.y * 1 / 6;
		}
		else if (index % 3 == 1)
		{
			result.y = CAMERA_RESOLUTION.y * 1 / 2;
		}
		else if (index % 3 == 2)
		{
			result.y = CAMERA_RESOLUTION.y * 5 / 6;
		}

		return result;
	}

	inline int EraserManager::PointToArea(cv::Point2i pos)
	{
		int x_index = 0;
		int y_index = 0;

		// 行の判定
		if (0 < pos.x && pos.x < CAMERA_RESOLUTION.x / 3)
		{
			x_index = 0;
		}
		else if (CAMERA_RESOLUTION.x / 3 < pos.x && pos.x < (CAMERA_RESOLUTION.x / 3) * 2)
		{
			x_index = 1;
		}
		else if ((CAMERA_RESOLUTION.x / 3) * 2 < pos.x && pos.x < CAMERA_RESOLUTION.x)
		{
			x_index = 2;
		}
		else
		{
			//std::cout << "[ERASER]x座標上は黒板にいません。" << std::endl;
		}

		// 行の判定
		if (0 < pos.y && pos.y < CAMERA_RESOLUTION.y * (1 / 3))
		{
			y_index = 0;
		}
		else if (CAMERA_RESOLUTION.y * (1 / 3) < pos.y && pos.y < CAMERA_RESOLUTION.y * (2 / 3))
		{
			y_index = 1;
		}
		else if (CAMERA_RESOLUTION.y * (2 / 3) && pos.y < CAMERA_RESOLUTION.y)
		{
			y_index = 2;
		}
		else
		{
			//std::cout << "[ERASER]y座標上は黒板にいません。" << std::endl;
		}

		// 現状だと(3×3なので)
		// [0][3][6]
		// [1][4][7]
		// [2][5][8]
		return (x_index * 3) + y_index;
	}
	inline void EraserManager::CulcurateArea()
	{
		for (int i = 0; i < 9; ++i)
		{
			m_areaWeight[i] = 0;
		}

		for (auto chalk : chalkPoints)
		{
			int area = PointToArea(chalk);
			m_areaWeight[area]++;
		}
	}
}