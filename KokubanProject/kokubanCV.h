#pragma once
#include <opencv2/opencv.hpp>


namespace kokubanCV {
	//�}�E�X���͗p�̃p�����[�^
	struct mouseParam {
		int x;
		int y;
		int event;
		int flags;
	};


	/// <summary>
	/// ����̃`���[�N�ŏ����ꂽ�ʒu�����
	/// ���̊O�`�̍��W��Ⴂ���͈͓̔��ɂ���2�l�������ۂɔ����ꏊ�̍��W��Ԃ�
	/// </summary>
	/// <param name="binary_img">�J���[�摜</param>
	/// <param name=" threshold">2�l������臒l</param>
	/// <param name="ksize">�t�B���^�������̏����P�ʂ̍s��̑傫�� 1�ȏ�̊��ݒ肷��</param>
	/// <returns>���������̍��Wpoint(x,y)��������vector</returns>
	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat image, int threshold = 128, int ksize = 7);

	/// <summary>
	/// ����2�l���֐�
	/// ���͉摜��jpg�Ȃ�܂Ƃ��ɓ���
	/// mp4��������
	/// png�͕ςɂȂ�i���Ԃ�z��ɓ����Ă��鏇�ԂƂ��v�f���Ⴄ�j
	/// </summary>
	/// <param name="img">�J���[�摜</param>
	/// <param name=" threshold">2�l������臒l</param>
	/// <param name="ksize">�t�B���^�������̏����P�ʂ̍s��̑傫�� 1�ȏ�̊��ݒ肷��</param>
	/// <returns>2�l�������摜</returns>
	cv::Mat binary(cv::Mat img, int threshold = 128, int ksize = 7);
	
	/// <summary>
	/// �w�肳�ꂽ4�_�œ����ϊ����s��
	/// �����ϊ���̐؂蔲���摜��Ԃ�
	///  �Q�l
    /// �����ϊ� https://cvtech.cc/homography/
    /// �}�E�X�擾 http://whitecat-student.hatenablog.com/entry/2016/11/09/223230
	/// </summary>
	cv::Mat clickPointPerspectiveTransformation(cv::Mat input_image, cv::Point2f click_point[4]);

	/// <summary>
	/// �摜�̓���4�_���N���b�N�Ŏw�肷��
	/// 4�_�͍���A�����A�E��A�E���̏��ŃN���b�N����
	/// �w�肳�ꂽ4�_�͈�����point�Ɋi�[
	/// </summary>
	void returnClickPoints(cv::Mat input_image, cv::Point2f point[4]);

	void mouseCallback(int event, int x, int y, int flags, void* userdata);
	cv::Mat maskOrange(cv::Mat input);

	cv::VideoCapture openVideoFile(std::string video_name);
	template <class Fn> void funcVideo(std::string video_name, std::string save_file_name, Fn fn);

	/// <summary>
	/// ��`�����o����
	/// ���o������`�ɐ���`�����摜��Ԃ�
	/// </summary>
	/// <param name="threshold_image">�摜</param>
	/// <returns>���̗̈�����o�������W</returns>
	std::vector<std::vector<cv::Point>> conto(cv::Mat img);

	/// <summary>
	/// �摜��2�l������
	/// opencv�̂��
	/// </summary>
	/// <param name="frame">�����Ώۂ̃J���[�摜</param>
	/// <param name="threshold">2�l����臒l</param>
	/// <param name="is_bright">�摜�����邢�Ƃ���true�ɂ���Ɨǂ�����</param>
	/// <returns>2�l�������摜</returns>
	cv::Mat color_to_binary(cv::Mat frame, int threshold = 128, bool is_bright = false);

	///<summary>
	///<para>�J�����Ɛڑ����ăt���[�����Ƃɏ���������</para>
	/// <para>camera open and some process every frame</para>
	///</summary>
	///<param name="device_nan">�J���f�o�C�X�̑I��</param>
	/// <param name="fn">�擾�����t���[���������Ɏ�菈��������֐�</param>
	template<class Fn> void run_capture_and_process(int device_nan, Fn fn);
}