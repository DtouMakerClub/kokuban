#ifndef __SerialWrapper__
#define __SerialWrapper__

#include <Windows.h>

//http://www.nyankobass.net/archives/133
class SerialWrapper
{
	HANDLE com_port;

public:
	// arg1 : COM�|�[�g�̔ԍ����w��
	// �w�肵���V���A���f�o�C�X���J��
	SerialWrapper(int comNum);

	// �J���Ă����V���A���f�o�C�X��j������
	virtual ~SerialWrapper();

	// arg1 : �`�����x[bps]
	// �V���A���ʐM�̏�����
	void begin(unsigned long speed);

	// arg1 : ���M����1byte
	// 1byte�̃f�[�^�𑗐M����
	// ���ۂɑ��M����byte����Ԃ�
	virtual size_t write(char byte);

	// arg1 : ���M����byte��
	// arg2 : ���M����byte��
	// ����byte�̃f�[�^����x�ɑ��M����
	// ���ۂɑ��M����byte����Ԃ�
	virtual size_t write(const char buf[], int len);

	// ��M�o�b�t�@�ɗ��܂���byte����Ԃ�
	virtual int available();

	// ��M�o�b�t�@����1byte�ǂݍ���
	// ���s�����ꍇ-1��Ԃ�
	virtual unsigned char read();

	// ��M�o�b�t�@���N���A����
	virtual void fflush();
};

#endif __SerialWrapper__