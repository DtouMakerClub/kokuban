#pragma once

#include <opencv2\opencv.hpp>

namespace Eraser 
{
	class EraserStateBase
	{
	public:
		EraserStateBase();
		~EraserStateBase();

		void Update();

		cv::Point2i nowPoint;
		std::vector<cv::Point2i> chalkPoints;			// 検出したチョークのピクセル
	private:
	protected:
		int PointToArea(cv::Point2i pos);
		cv::Point2i AreaToPoint(int index);
	};
}
