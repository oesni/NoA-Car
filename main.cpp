
#include <stdio.h>
#include <iostream>
#include <string>
#include "SerialComm.h"

using namespace std;


int main()
{
	unsigned char buffer[2], buf_char; int buf_int, _a, _b;
	CSerialComm serialComm; //SerialComm 객체 생성
	CSerialComm bluetoothmod;
	if (!serialComm.connect("COM5")) {
		cout << "Arduino Uno 보드 연결 실패" << endl; return -1;
	}
	else
		cout << "Arduino Uno 보드 연결 성공" << endl;
	if (!bluetoothmod.connect("COM8")) {
		cout << "JMOD-128-1 연결 실패" << endl; return -1;
	}
	else
		cout << "JMOD-128-1 연결 성공" << endl;
	char test; char testrx;

	while (1) { //오픈에 성공한 경우 sendCommand()를 통해 계속적으로 데이터를 전송한다. 전송에 실패 할 경우 failed 메시지를 출력한다.
		if (bluetoothmod.recvCommandbyBluetooth(testrx)) {
			cout << "전달받은 값 : " << testrx << endl;
			if (!serialComm.sendCommand(testrx))
			{
				cout << "send command failed" << endl;
			}
			else
				cout << "send Command success" << endl;
		}
		else
			cout << "recieve command from bluetooth failed" << endl;

		/*for (int i = 0; i < 4; i++) {
			cin >> test;
			if (!bluetoothmod.sendCommandbyBluetooth(test))
			{
				cout << "send command failed" << endl;
			}
			else
				cout << "send Command success" << endl;
		}
		for (int i = 0; i < 6; i++) {
			if (bluetoothmod.recvCommandbyBluetooth(testrx))
				cout << "전달받은 값 : " << testrx << endl;
			else
				cout << "recieve command failed" << endl;
		}*/
		//cout << "조향값 입력" << endl;
		//cin >> buf_int;
		////cout << "구동 값 입력(u, s, h, g, d)" << endl;
		////cin >> buf_char;
		//_a = buf_int % 256; 
		//buffer[0] = (char)_a;
		//_b = (buf_int / 256);
		//buffer[1] = (char) _b;
		//if (!serialComm.sendCommand(buffer[1]))
		//{
		//	cout << "send command failed" << endl;
		//}
		//else
		//	cout << "send Command success" << endl;
		//if (!serialComm.sendCommand(buffer[0]))
		//{
		//	cout << "send command failed" << endl;
		//}
		//else
		//	cout << "send Command success" << endl;
		//if (!serialComm.sendCommand('g'))
		//{
		//	cout << "send command failed" << endl;
		//}
		//else
		//	cout << "send Command success" << endl;

	}


	serialComm.disconnect(); //작업이 끝나면 포트를 닫는다
	bluetoothmod.disconnect();


	cout << "end connect" << endl;
	return 0;

}
