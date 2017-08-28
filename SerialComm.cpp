#ifndef SERIALCOMM_CPP
#define SERIALCOMM_CPP
#include "SerialComm.h"


CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}


int CSerialComm::connect(char* portNum)
{
	if (!serial.OpenPort(portNum)) //포트를 오픈하고 오픈에 실패하였으면 fail을 반환한다.
		return RETURN_FAIL;
	//보드레이트 : 115200bps, 1데이터에 8bit, Parity,비트 사용안함, 1stopBit
	serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT); //포트 기본값을 설정한다.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout값 설정

	return RETURN_SUCCESS;
}


int CSerialComm::sendCommand(char pos) //데이터를 전송하는 함수
{
	if (serial.WriteByte(pos)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}
int CSerialComm::recvCommand(char* &pos) {
	BYTE* t;
	if (serial.ReadByte(t, 3)) {
		pos = (char*)t;
		return RETURN_SUCCESS;
	}
	else
		return RETURN_FAIL;
}
int CSerialComm::recvCommand(char &one) {
	BYTE t; one = 0x00;
	if (serial.ReadByte(t)) {
		one += t;
		return RETURN_SUCCESS;
	}
	else
		return RETURN_FAIL;
}
void CSerialComm::disconnect() //포트를 다 쓰고 난뒤 닫는 함수
{
	serial.ClosePort();
}

int CSerialComm::sendCommandbyBluetooth(char data) {
	if (serial.WriteByte(data)) {
		serial.WriteByte(0x0d);
		serial.WriteByte(0x0a);
		return RETURN_SUCCESS;
	}
	else
		return RETURN_FAIL;
}

int CSerialComm::recvCommandbyBluetooth(char &data) {
	BYTE t; data = 0x00;
	if (serial.ReadByte(t)) {
		data += t;
		serial.ReadByte(t);
		serial.ReadByte(t);
		return RETURN_SUCCESS;
	}
	else
		return RETURN_FAIL;
}
#endif
