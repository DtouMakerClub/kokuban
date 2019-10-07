#pragma once

#include <opencv2\opencv.hpp>

namespace Eraser 
{
	enum EraserStatus
	{
		AreaMove = 0,
		PointMove = 1,
	};

	class EraserStateBase
	{
	public:
		EraserStateBase();
		~EraserStateBase();

		void Update();

		std::vector<cv::Point2i> chalkPoints;			// 検出したチョークのピクセル
	private:
		// 状態の更新処理
		void UpdateState();

		int m_weight;									// 現在いるエリアの重み
		EraserStatus m_status;							// 移動の方法
	protected:
		// 座標→エリアインデックスの変換
		int PointToArea(cv::Point2i pos);
		// エリアインデックス→座標の変換
		cv::Point2i AreaToPoint(int index);
		// エリアの重みを計算
		void CulcurateArea();

		int m_areaWeight[9];				// 各エリアの重み
		int m_nowAreaIndex;					// 自分の位置エリア
		cv::Point2i nowPoint;				// 自分の位置座標
		int m_targetIndex;					// 目標位置のエリア
		cv::Point2i m_targetPoint;			// 目標位置の座標


	};
}
