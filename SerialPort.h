#pragma once
#include <Windows.h>
#include <atlstr.h>

#define BUFFER_SIZE 128

class CSerialPort
{
public:
	CSerialPort(void);
	virtual ~CSerialPort(void);

private:
	HANDLE  m_hComm;
	DCB     m_dcb;
	COMMTIMEOUTS m_CommTimeouts;
	bool    m_bPortReady;
	bool    m_bWriteRC;
	bool    m_bReadRC;
	DWORD   m_iBytesWritten;
	DWORD   m_iBytesRead;
	DWORD   m_dwBytesRead;

public:
	//��Ʈ�� �ݴ� �Լ�
	void ClosePort();
	//��Ʈ���� �����͸� �о���� �Լ�
	bool ReadByte(BYTE &resp);
	bool ReadByte(BYTE* &resp, UINT size);
	//��Ʈ�� �����͸� �����ϴ� �Լ�
	bool WriteByte(char bybyte);
	//��Ʈ�� ���� �Լ�
	bool OpenPort(CString portname);
	//�����Ʈ���� TimeOut�� �����ϴ� �Լ�
	bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout,
		DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant,
		DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant);
	//��Ʈ�� �ʱ�ȭ �����ϴ� �Լ�
	bool ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity,
		BYTE  Parity, BYTE StopBits);
};