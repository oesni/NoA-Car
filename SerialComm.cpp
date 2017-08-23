#include "SerialComm.h"


CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}


int CSerialComm::connect(char* portNum)
{
	if (!serial.OpenPort(portNum)) //��Ʈ�� �����ϰ� ���¿� �����Ͽ����� fail�� ��ȯ�Ѵ�.
		return RETURN_FAIL;
	//���巹��Ʈ : 115200bps, 1�����Ϳ� 8bit, Parity,��Ʈ ������, 1stopBit
	serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT); //��Ʈ �⺻���� �����Ѵ�.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout�� ����

	return RETURN_SUCCESS;
}


int CSerialComm::sendCommand(char pos) //�����͸� �����ϴ� �Լ�
{
	if (serial.WriteByte(pos)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}
int CSerialComm::recvCommand(char pos) {
	if (serial.ReadByte((BYTE&) pos))
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}
void CSerialComm::disconnect() //��Ʈ�� �� ���� ���� �ݴ� �Լ�
{
	serial.ClosePort();
}