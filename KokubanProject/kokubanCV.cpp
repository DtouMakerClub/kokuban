#include "kokubanCV.h"

namespace kokubanCV {

	std::vector<cv::Point>  pulledOutChalkOnKokuban(cv::Mat image, int threshold)
	{
		int width = image.cols;
		int height = image.rows;
		std::vector<cv::Point> chalkPoints;
		cv::Vec3b zero = (0, 0, 0);
		cv::Mat_<cv::Vec3b> img(image);

		for (int y = 0; y <height; ++y) {
			//y���𑖍�
			cv::Vec3b* src = image.ptr<cv::Vec3b>(y);
			for (int x = 0; x < width; ++x) {
				//x���𑖍�
				if (img.at<cv::Vec3b>(y, x)[0] * 0.299 + img.at<cv::Vec3b>(y, x)[1] * 0.587 + img.at<cv::Vec3b>(y, x)[2] * 0.114 > threshold) {
					chalkPoints.push_back(cv::Point(x,y));
					//std::cout << chalkPoints.back() << std::endl;
				}
			}
		}
		std::cout << chalkPoints.size() <<   std::endl;

		//���o�����_�Q�̍��W��Ԃ�
		return chalkPoints;
	}


	cv::Mat binary(cv::Mat img, int threshold)
	{
		int width = img.cols;
		int height = img.rows;
		cv::Mat binary_img = img;

		for (int y = 0; y < height -10; ++y) {
			//y���𑖍�
			//cv::Vec3b* src = img.ptr<cv::Vec3b>(y);

			for (int x = 0; x < width; ++x) {
				//x���𑖍�
				if (img.at<cv::Vec3b>(y, x)[0] * 0.299 +  img.at<cv::Vec3b>(y, x)[1] * 0.587 +  img.at<cv::Vec3b>(y, x)[2]  * 0.114 > threshold) {
					binary_img.at<cv::Vec3b>(y, x)[0] = 255;
					binary_img.at<cv::Vec3b>(y, x)[1] = 255;
					binary_img.at<cv::Vec3b>(y, x)[2] = 255;
				}else {
					binary_img.at<cv::Vec3b>(y, x)[0] = 0;
					binary_img.at<cv::Vec3b>(y, x)[1] = 0;
					binary_img.at<cv::Vec3b>(y, x)[2] = 0;
				}
			}
		}

		//���o�����_�Q�̍��W��Ԃ�
		return binary_img;
	}

	cv::Mat clickPointPerspectiveTransformation(cv::Mat input_img)
	{
		kokubanCV::mouseParam mouse;
		cv::Mat frame;
		cv::Size size = { int(input_img.cols * 0.25), int(input_img.rows * 0.25)};
		cv::resize(input_img, frame, size);
		cv::imshow("input_image", frame);//�摜��\��
		cv::setMouseCallback("input_image", kokubanCV::mouseCallback, &mouse);
		int width = frame.cols;
		int height = frame.rows;

		cv::Point2f src_pt[4] = {};
		const cv::Point2f dst_pt[4] = {
					cv::Point2f(50,							 50),
					cv::Point2f(50,							 50 + height - 1),
					cv::Point2f(50 + width - 1,       50),
					cv::Point2f(50 + width - 1,       50 + height - 1) };

		int i = 0;
		cv::Mat dst = cv::Mat::zeros(height + 100, width + 100, CV_8UC3);

		std::cout << "click point 4 times" << std::endl;
		while (1) {
			cv::waitKey(2);
			//���N���b�N�������W���擾
			if (mouse.event == cv::EVENT_LBUTTONDOWN && i <= 4) {
				//�N���b�N��̃}�E�X�̍��W���o��
				std::cout << mouse.x << " , " << mouse.y << std::endl;
				// ����A�����A�E��A�E���̏��œ���
				src_pt[i] = cv::Point2f(mouse.x, mouse.y);
				++i;
				cv::waitKey(200);
			}
			//4�_�N���b�N�ŏI��
			else if (i >= 4) {
				cv::Mat homography_matrix = getPerspectiveTransform(src_pt, dst_pt);
				warpPerspective(frame, dst, homography_matrix, dst.size());
				imshow("result", dst);
				cv::waitKey(500);
				break;
			}
		}
		cv::destroyAllWindows();
		return dst;
	}


	void mouseCallback(int event, int x, int y, int flags, void* userdata)
	{
		mouseParam* ptr = static_cast<mouseParam*> (userdata);

		ptr->x = x;
		ptr->y = y;
		ptr->event = event;
		ptr->flags = flags;
	}


	cv::VideoCapture openVideoFile(std::string video_name)
	{
		cv::VideoCapture cap;
		cap.open(video_name);

		if (cap.isOpened() == false) {
			std::cout << video_name << "  cant open " << std::endl;
			return -1;
		}
		return cap;
	}

	template <class Fn> void funcVideo(std::string video_name, std::string save_file_name, Fn fn)
	{
		cv::VideoCapture cap = openVideoFile(video_name);
		// �쐬���铮��t�@�C���̏��ݒ�
		int    fourcc, width, height;
		double fps;

		width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);	// �t���[���������擾
		height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);	// �t���[���c�����擾
		fps = cap.get(cv::CAP_PROP_FPS);					// �t���[�����[�g���擾
		fourcc = cv::VideoWriter::fourcc('M', 'P', '4', 'V');	// �G���R�[�h�`�����w��
		// * �G���R�[�h�`�� "XVID" = AVI, "MP4V" = MPEG4, "WMV1" = WMV

		// ����t�@�C���������o�����߂̃I�u�W�F�N�g��錾����
		cv::VideoWriter writer;
		writer.open(save_file_name, fourcc, fps, cv::Size(width, height));

		cv::Mat frame, dst;

		while (true) {
			
			cap >> frame;

			if (frame.empty() == true) {
				break;
			}

			//��������������
			dst = fn(frame);

			cv::imshow("�ϊ���", frame);

			writer << dst;
			cv::waitKey(1);
		}

		writer.release();
		std::cout << "end " << std::endl;
	}
}
