#include "EraserManager.h"

namespace Eraser
{
	EraserManager::EraserManager()
	{
		m_state = EraserMoveState::READY;
		
		m_eraserPos = cv::Point2i(CAMERA_RESOLUTION.x / 2, CAMERA_RESOLUTION.y / 2);
		m_prevPos = m_eraserPos;
		serialCommnad = new KokubanSerial(4);

		// XYステージのキャリブレーション命令
		serialCommnad->start();
	}

	EraserManager::~EraserManager()
	{
		delete(serialCommnad);
	}

	void EraserManager::Update()
	{

		// 目標位置の更新処理
		UpdateMove();

		// 各状態の更新処理
		UpdateState();

		// 値の送信
		//SendTargetPosition();

		// デバッグ用環境
		//DebugSimulate();

	}

	void EraserManager::UpdateMove()
	{
		switch (m_state)
		{
		case EraserMoveState::READY:
			return;
			break;
		case EraserMoveState::AREA:
			// 移動するためのエリアを選出
			m_targetPoint = AreaToPoint(m_targetIndex);
			break;
		case EraserMoveState::POINT:
			// 自身から最も近い点を取得する
			m_targetPoint = FindNearest(m_nowAreaIndex);
			break;
		default:
			break;
		}

		cv::Point ratio = serialCommnad->readMessage(m_eraserPos);
		m_eraserPos = RatioToPoint(ratio);
		std::cout << m_eraserPos << std::endl;
	}

	inline void EraserManager::DebugDraw(cv::Mat img)
	{
		//img = cv::Scalar(255, 255, 255);

		// チョークリーナー
		cv::Point p1(20, 40);
		cv::Scalar color;

		switch (m_state)
		{
		case EraserMoveState::AREA:
			color = cv::Scalar(255, 0, 0);
			break;
		case EraserMoveState::POINT:
			color = cv::Scalar(0, 0, 255);
			break;
		default:
			color = cv::Scalar(0, 255, 0);
			break;
		}

		// 黒板消しの位置
		//cv::rectangle(img, m_eraserPos, m_eraserPos + p1, cv::Scalar(0, 0, 0), 10, cv::LINE_4);
		cv::circle(img, m_eraserPos, 10, cv::Scalar(0, 0, 255));

		// 目的地
		cv::rectangle(img, m_targetPoint, m_targetPoint + cv::Point(5, 5), cv::Scalar(255, 0, 255), 1, cv::LINE_4);

		// エリアわけライン
		//cv::Point x1_line(CAMERA_RESOLUTION.x / 3, 0);
		//cv::Point x2_line((CAMERA_RESOLUTION.x / 3) * 2, 0);
		//cv::Point y1_line(0, CAMERA_RESOLUTION.y / 3);
		//cv::Point y2_line(0, (CAMERA_RESOLUTION.y / 3) * 2);

		//cv::line(img, x1_line, x1_line + cv::Point(0, CAMERA_RESOLUTION.y), cv::Scalar(255, 0, 0), 5, cv::LINE_AA);
		//cv::line(img, x2_line, x2_line + cv::Point(0, CAMERA_RESOLUTION.y), cv::Scalar(255, 0, 0), 5, cv::LINE_AA);
		//cv::line(img, y1_line, y1_line + cv::Point(CAMERA_RESOLUTION.x, 0), cv::Scalar(255, 0, 0), 5, cv::LINE_AA);
		//cv::line(img, y2_line, y2_line + cv::Point(CAMERA_RESOLUTION.x, 0), cv::Scalar(255, 0, 0), 5, cv::LINE_AA);

		// チョーク
		//for (auto chalk : chalks)
		//{
		//	cv::rectangle(img, chalk, chalk + p1, cv::Scalar(0, 0, 0), 1, cv::LINE_4);
		//}


		//std::cout << "pos:" << m_eraserPos << std::endl;
		//std::cout << "area:" << m_nowAreaIndex << std::endl;
		//std::cout << "target:" << m_targetIndex << std::endl;

		cv::imshow("test", img);
		cv::waitKey(1);
	
	}

	inline void EraserManager::DebugSimulate()
	{
		m_nowAreaIndex = PointToArea(m_eraserPos);
		cv::Point2i vec = m_targetPoint - m_eraserPos;
		vec /= cv::norm(vec);
		m_prevPos = m_eraserPos;
		m_eraserPos += vec * 2;
	}

	inline cv::Point2i EraserManager::FindNearest(int index)
	{
		cv::Point2i target;
		float minDist = 100000.0f;
		float dist = 0.0f;

		for (auto chalk : chalkPoints)
		{
			int area = PointToArea(chalk);
			/*if (area == m_nowAreaIndex)
			{*/
				dist = GetDistance(chalk, m_eraserPos);
				if (minDist > dist)
				{
					target = chalk;
					minDist = dist;
				}
			//}
		}

		return target;
	}
	inline void EraserManager::UpdateState()
	{
		if (m_state == EraserMoveState::READY)
		{
			if (serialCommnad->checkRead())
			{
				m_state = EraserMoveState::POINT;
			}
		}
		else
		{
			// 現エリアの重みが小さくなったら
			//if (m_areaWeight[m_nowAreaIndex] < ((CAMERA_RESOLUTION.x * CAMERA_RESOLUTION.y) / 9) * MOVE_RATE)
			// ある一定の近さにチョークがなかったら
			if (GetDistance(m_targetPoint, m_eraserPos) > 200.0f)
			{
				m_state = EraserMoveState::AREA;
				//CulcurateArea();
			}

			// エリアの移動が完了したら
			else if (m_nowAreaIndex == m_targetIndex)
			{
				m_state = EraserMoveState::POINT;
				//CulcurateArea();
				m_targetIndex = std::rand() % 9;//TargetAreaCheck();
			}
		}
	}

	inline void EraserManager::SendTargetPosition()
	{
		cv::Point2i sendPoint;
		sendPoint = PointToRatio(m_targetPoint);
		
		// 送信するときに自分の位置に近すぎる座標は省く

		serialCommnad->sendMessage(sendPoint.x, sendPoint.y);

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
			result.x = CAMERA_RESOLUTION.x / 6;
		}
		else if (index < 6)
		{
			result.x = CAMERA_RESOLUTION.x / 2;
		}
		else if (index < 9)
		{
			result.x = (CAMERA_RESOLUTION.x / 6) * 5;
		}

		if (index % 3 == 0)
		{
			result.y = CAMERA_RESOLUTION.y / 6;
		}
		else if (index % 3 == 1)
		{
			result.y = CAMERA_RESOLUTION.y / 2;
		}
		else if (index % 3 == 2)
		{
			result.y = (CAMERA_RESOLUTION.y / 6) * 5;
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
		if (0 < pos.y && pos.y < CAMERA_RESOLUTION.y / 3)
		{
			y_index = 0;
		}
		else if (CAMERA_RESOLUTION.y / 3 < pos.y && pos.y < (CAMERA_RESOLUTION.y / 3) * 2)
		{
			y_index = 1;
		}
		else if ((CAMERA_RESOLUTION.y / 3) * 2 && pos.y < CAMERA_RESOLUTION.y)
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

		// 各エリアの白ドットの数を数えて重みとしている
		for (auto chalk : chalkPoints)
		{
			int area = PointToArea(chalk);
			m_areaWeight[area]++;
		}
	}
	inline cv::Point2i EraserManager::PointToRatio(cv::Point2i pos)
	{
		cv::Point2i result;

		// 送信用の仕様が0~255なのでそれに合わせている
		result.x = (pos.x / CAMERA_RESOLUTION.x) * 255;
		result.y = (pos.y / CAMERA_RESOLUTION.y) * 255;

		return result;
	}

	inline cv::Point2i EraserManager::RatioToPoint(cv::Point2i ratio)
	{
		cv::Point2f result;

		// 受信用の値が0~255なのでピクセル表記に変換する
		result.x = ratio.x * CAMERA_RESOLUTION.x / 255.0f;
		result.y = ratio.y * CAMERA_RESOLUTION.y / 255.0f;

		if (result.x > CAMERA_RESOLUTION.x)
		{
			result.x = CAMERA_RESOLUTION.x;
		}

		if (result.y > CAMERA_RESOLUTION.y)
		{
			result.y = CAMERA_RESOLUTION.y;
		}

		return result;
	}

	inline float EraserManager::GetDistance(cv::Point p1, cv::Point p2)
	{
		return static_cast<float>(cv::norm(p1 - p2));;
	}
}