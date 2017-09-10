#pragma once
#include "SerialPort.h"

#define RETURN_SUCCESS 1
#define RETURN_FAIL 0


class CSerialComm
{
public:
	CSerialComm();
	~CSerialComm();

	CSerialPort	serial;
	int		connect(char* _portNum);
	int		sendCommand(char pos);
	int recvCommand(char* &pos);
	int recvCommand(char &one);
	void		disconnect();

	int sendCommandbyBluetooth(char data);
	int recvCommandbyBluetooth(char &data);
};