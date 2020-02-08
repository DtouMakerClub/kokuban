#include "SerialWrapper.h"

#include <tchar.h>

SerialWrapper::SerialWrapper(int comNum)
{
	TCHAR com[10];
	_stprintf_s(com, 10, _T("COM%d"), comNum);

	// �V���A���|�[�g���J��
	com_port = CreateFile(com, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
}

// �J���Ă����V���A���f�o�C�X��j������
 SerialWrapper::~SerialWrapper()
{
	// �V���A���|�[�g�����
	CloseHandle(com_port);
}

void SerialWrapper::begin(unsigned long speed)
{
	DCB dcb; // �V���A���|�[�g�̍\����񂪓���\����
	GetCommState(com_port, &dcb); // ���݂̐ݒ�l��ǂݍ���

	dcb.BaudRate = speed; // ���x
	dcb.ByteSize = 8; // �f�[�^��
	dcb.Parity = NOPARITY; // �p���e�B
	dcb.StopBits = ONESTOPBIT; // �X�g�b�v�r�b�g��
	dcb.fOutxCtsFlow = FALSE; // ���M��CTS�t���[
	dcb.fRtsControl = RTS_CONTROL_ENABLE; // RTS�t���[

	// �ύX�����ݒ�l����������
	SetCommState(com_port, &dcb);
}

size_t SerialWrapper::write(char byte)
{
	char* sentData = &byte; // ���M����1byte
	DWORD numberOfPut; // ���ۂɑ��M����byte��

	// �|�[�g�֑��M
	WriteFile(com_port, sentData, 1, &numberOfPut, NULL);

	return numberOfPut;
}

size_t SerialWrapper::write(const char buf[], int len)
{
	DWORD lengthOfSent = len; // ���M����byte��
	DWORD numberOfPut; // ���ۂɑ��M����byte��

	// �|�[�g�֑��M
	WriteFile(com_port, buf, lengthOfSent, &numberOfPut, NULL);

	return numberOfPut;
}

int SerialWrapper::available()
{
	//��M�f�[�^���𒲂ׂ�
	DWORD errors;
	COMSTAT comStat;
	ClearCommError(com_port, &errors, &comStat);

	int lengthOfRecieved = comStat.cbInQue; // ��M�������b�Z�[�W�����擾����

	return lengthOfRecieved;
}

unsigned char SerialWrapper::read()
{
	//��M�f�[�^���Ȃ��ꍇ�͓ǂݍ��܂Ȃ�
	if (available() < 1)
	{
		return 0;
	}

	unsigned char buf[1]; // ��M�f�[�^�i�[�p
	DWORD numberOfRead; // ���ۂɎ�M�����o�C�g��

	//�f�[�^��M
	bool result = ReadFile(com_port, buf, 1, &numberOfRead, NULL);

	if (result == FALSE)
	{
		return 0;
	}
	else
	{
		return buf[0];
	}
}

void SerialWrapper::fflush()
{
	unsigned char buf[1]; // ��M�f�[�^�i�[�p
	DWORD numberOfRead; // ���ۂɎ�M�����o�C�g��
	
	while(available()){
		ReadFile(com_port, buf, 1, &numberOfRead, NULL);
	}
}