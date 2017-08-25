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
	//포트를 닫는 함수
	void ClosePort();
	//포트에서 데이터를 읽어오는 함수
	bool ReadByte(BYTE &resp);
	bool ReadByte(BYTE* &resp, UINT size);
	//포트로 데이터를 전송하는 함수
	bool WriteByte(char bybyte);
	//포트를 여는 함수
	bool OpenPort(CString portname);
	//통신포트에서 TimeOut을 설정하는 함수
	bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout,
		DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant,
		DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant);
	//포트를 초기화 설정하는 함수
	bool ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity,
		BYTE  Parity, BYTE StopBits);
};
