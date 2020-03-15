#pragma once
#include <opencv2/opencv.hpp>


namespace kokubanCV {
	//マウス入力用のパラメータ
	struct mouseParam {
		int x;
		int y;
		int event;
		int flags;
	};


	/// <summary>
	/// 黒板上のチョークで書かれた位置を特定
	/// 黒板の外形の座標を貰い黒板の範囲内にある2値化した際に白い場所の座標を返す
	/// </summary>
	/// <param name="binary_img">カラー画像</param>
	/// <param name=" threshold">2値化時の閾値</param>
	/// <param name="ksize">フィルタ処理時の処理単位の行列の大きさ 1以上の奇数を設定する</param>
	/// <returns>白い部分の座標point(x,y)が入ったvector</returns>
	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat image, int threshold = 128, int ksize = 7);

	/// <summary>
	/// 自作2値か関数
	/// 入力画像がjpgならまともに動く
	/// mp4もいけた
	/// pngは変になる（たぶん配列に入っている順番とか要素が違う）
	/// </summary>
	/// <param name="img">カラー画像</param>
	/// <param name=" threshold">2値化時の閾値</param>
	/// <param name="ksize">フィルタ処理時の処理単位の行列の大きさ 1以上の奇数を設定する</param>
	/// <returns>2値化した画像</returns>
	cv::Mat binary(cv::Mat img, int threshold = 128, int ksize = 7);
	
	/// <summary>
	/// 指定された4点で透視変換を行う
	/// 透視変換後の切り抜き画像を返す
	///  参考
    /// 透視変換 https://cvtech.cc/homography/
    /// マウス取得 http://whitecat-student.hatenablog.com/entry/2016/11/09/223230
	/// </summary>
	cv::Mat clickPointPerspectiveTransformation(cv::Mat input_image, cv::Point2f click_point[4]);

	/// <summary>
	/// 画像の内の4点をクリックで指定する
	/// 4点は左上、左下、右上、右下の順でクリックする
	/// 指定された4点は引数のpointに格納
	/// </summary>
	void returnClickPoints(cv::Mat input_image, cv::Point2f point[4]);

	void mouseCallback(int event, int x, int y, int flags, void* userdata);
	cv::Mat maskOrange(cv::Mat input);

	cv::VideoCapture openVideoFile(std::string video_name);
	template <class Fn> void funcVideo(std::string video_name, std::string save_file_name, Fn fn);

	/// <summary>
	/// 矩形を検出する
	/// 検出した矩形に線を描いた画像を返す
	/// </summary>
	/// <param name="threshold_image">画像</param>
	/// <returns>黒板の領域を検出した座標</returns>
	std::vector<std::vector<cv::Point>> conto(cv::Mat img);

	/// <summary>
	/// 画像を2値化する
	/// opencvのやつ
	/// </summary>
	/// <param name="frame">処理対象のカラー画像</param>
	/// <param name="threshold">2値化の閾値</param>
	/// <param name="is_bright">画像が明るいときはtrueにすると良いかも</param>
	/// <returns>2値化した画像</returns>
	cv::Mat color_to_binary(cv::Mat frame, int threshold = 128, bool is_bright = false);

	///<summary>
	///<para>カメラと接続してフレームごとに処理をする</para>
	/// <para>camera open and some process every frame</para>
	///</summary>
	///<param name="device_nan">開くデバイスの選択</param>
	/// <param name="fn">取得したフレームを引数に取り処理をする関数</param>
	template<class Fn> void run_capture_and_process(int device_nan, Fn fn);
}