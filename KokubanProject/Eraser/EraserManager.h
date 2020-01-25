#pragma once

#include <opencv2\opencv.hpp>

#include "EraserStateBase.h"

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
		};

		EraserManager();
		~EraserManager();

		void Update();

		void UpdateMove();
		void DebugDraw(cv::Mat img);

		cv::Point2i const CAMERA_RESOLUTION = cv::Point2i(1478, 1108);

		int const HEIGHT_SECTION = 3;
		int const WIDTH_SECTION = 3;

		cv::Point2i GetEraserPos() { return m_eraserPos; }
		int GetAreaIndex() { return m_nowAreaIndex; }
		std::vector<cv::Point2i> GetChalk() { return chalkPoints; }

		std::vector<cv::Point2i> chalkPoints;			// 検出したチョークのピクセル
	private:

		// 移動するエリアの決定
		int TargetAreaCheck();
		// 自身の位置に最近傍の点を取得する
		cv::Point2i FindNearest(int index);
		void UpdateState();
		// 現在位置の受信(xyステージとの)
		cv::Point2i ReceivePosition();
		// 目標位置の送信
		bool SendTargetPosition();
		// エリアインデックス→座標の変換
		cv::Point2i AreaToPoint(int index);
		// 座標→エリアインデックスの変換
		int PointToArea(cv::Point2i pos);
		// エリアの重みを計算
		void CulcurateArea();

		int m_areaWeight[9];				// 各エリアの重み
		int m_nowAreaIndex;					// 自分の位置エリア
		cv::Point2i m_eraserPos;				// 自分の位置座標
		int m_targetIndex;					// 目標位置のエリア
		cv::Point2i m_targetPoint;			// 目標位置の座標

		// 現在いるエリアの重み
		int m_nowAreaWeight;
		// 状態管理
		EraserMoveState m_state = EraserMoveState::READY;
	};
}
