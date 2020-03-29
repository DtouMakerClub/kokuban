#pragma once

#include <opencv2\opencv.hpp>

#include "EraserStateBase.h"
#include "../KokubanSerial.h"

namespace Eraser 
{
	class EraserManager
	{
	public:
		enum class EraserMoveState
		{
			READY,
			AREA,
			POINT,
			PAUSE,
		};

		EraserManager();
		~EraserManager();

		void Update();
		void InputKey();

		void UpdateMove();
		void DebugDraw(cv::Mat img);
		void DebugSimulate(cv::Point target);
		void DebugSend(cv::Point pos);

		cv::Point2i const CAMERA_RESOLUTION = cv::Point2i(640, 480);
		// 上の解像度だと100ピクセルぐらいがちょうどいいので0.1%	
		float const MOVE_RATE = 0.001f;					

		int const HEIGHT_SECTION = 3;
		int const WIDTH_SECTION = 3;

		KokubanSerial* serialCommnad = nullptr;

		cv::Point2i GetEraserPos() { return m_eraserPos; }
		int GetAreaIndex() { return m_eraserAreaIndex; }
		std::vector<cv::Point2i> GetChalk() { return chalkPoints; }

		std::vector<cv::Point2i> chalkPoints;			// 検出したチョークのピクセル
	private:
		// 移動するエリアの決定
		int TargetAreaCheck();
		// 自身の位置に最近傍の点を取得する
		cv::Point2i FindNearest();
		void UpdateState();
		// 目標位置の送信
		void SendTargetPosition();
		// エリアインデックス→座標の変換
		cv::Point2i AreaToPoint(int index);
		// 座標→エリアインデックスの変換
		int PointToArea(cv::Point2i pos);
		// エリアの重みを計算
		void CulcurateArea();
		// 指令値を割合に変換する
		cv::Point2i PointToRatio(cv::Point2i pos);
		// 送られてきた割合をピクセルに変換する
		cv::Point2i RatioToPoint(cv::Point2i ratio);
		// 2点間の距離を求める
		float GetDistance(cv::Point p1, cv::Point p2);

		int m_areaWeight[9];				// 各エリアの重み
		int m_eraserAreaIndex;					// 自分の位置エリア
		cv::Point2i m_eraserPos;				// 自分の位置座標
		cv::Point2i m_prevPos;				// 自分の位置座標
		int m_targetIndex;					// 目標位置のエリア
		cv::Point2i m_targetPoint;			// 目標位置の座標
		cv::TickMeter m_timer;

		// 現在いるエリアの重み
		int m_nowAreaWeight;
		// 状態管理
		EraserMoveState m_state = EraserMoveState::READY;
	};
}
