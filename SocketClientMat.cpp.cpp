#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define MAX_DATA_LENGTH 255

#define AMB_STATE_0 0
#define AMB_STATE_1 1
#define AMB_STATE_2 2
#define AMB_STATE_3 3

#define BLACK_STATE_0 4
#define BLACK_STATE_1 5



#include "WinsockMatTransmissionClient.h"
#include "Information.h"
#include "SerialPort.h"

//String for getting the output from arduino
char output[MAX_DATA_LENGTH];

/*Portname must contain these backslashes, and remember to
replace the following com port*/
char port_name[] = "COM6";

//String for incoming data
char incomingData[MAX_DATA_LENGTH];

int main()
{
	FILE *serPort = fopen(port_name, "r");
	if (serPort == NULL){
		cout << "atmega connection error!" << endl;
		return 0;
	}
	else{
		cout << "ok! atmega connection done" << endl;
	}
	int state = AMB_STATE_3;
	int socket_num;
	int bytesSent;
	information I;
	
//	SerialPort arduino(port_name);
//	if (arduino.isConnected()) cout << "Connection Established" << endl;

//	else cout << "ERROR, check port name";
	
	WinsockMatTransmissionClient socketMat;
	if (socketMat.socketConnect("165.246.235.92", 23957) < 0)
	{
		return 0;
	}

	socket_num = (int)socketMat.get_SOCKET();

	cv::VideoCapture capture(0);
	cv::Mat image;


	


	for (;;)
	{
		I.set_time();
		
		if (state == AMB_STATE_0){
			I.set_sender_id('A');
			cout << "*************AMB_STATE_0*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

			

			recv(socket_num, (char*)&I, sizeof(I), 0);

			if (I.get_packet_id() == 'A'){
				state == AMB_STATE_1;
			}
		}
		else if (state == AMB_STATE_1){
			I.set_sender_id('A');
			cout << "*************AMB_STATE_1*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

			//여기에 방해차량 번호판을 인식하는 코드가 필요함
			//주행코드도 필요함
			//if(방해차량 번호판 옳다면) {
			//	I.set_packet_id('D');
			//	send(socket_num, (char*)&I, sizeof(I), 0)
			//}
			recv(socket_num, (char*)&I, sizeof(I), 0);
			if (I.get_packet_id() == 'C'){
				state = AMB_STATE_2;
			}
		}
		else if (state == AMB_STATE_2){
			I.set_sender_id('A');
			cout << "*************AMB_STATE_2*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

			//여기에 사고차량 번호판을 인식하는 코드가 필요함
			//주행 코드도 필요함
			//if(방해차량 번호판 옳다면) {
			//	I.set_packet_id('F');
			//	send(socket_num, (char*)&I, sizeof(I), 0)
			//  state == AMB_STATE_3
			//}
			

		}
		else if (state == AMB_STATE_3){
			I.set_sender_id('A');
			cout << "*************AMB_STATE_3*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

		
			if (!capture.isOpened())
				return 0;
			capture >> image;
			if (image.empty())
				return 0;
			socketMat.transmit(image);
			imshow("original_video", image);
			cvWaitKey(27);
			send(socket_num, (char*)&I, sizeof(I), 0);

			//if 사고차량과 일정거리가 되면
			//I.set_packet_id('G');
			//send(socket_num, (char*)&I, sizeof(I), 0);



		}
		else if (state == BLACK_STATE_0){
			I.set_sender_id('B');
			cout << "*************BLACK_STATE_0*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

		
			recv(socket_num, (char*)&I, sizeof(I), 0);
			if (I.get_packet_id() == 'B'){
				//여기에 주차 주행 코드를 넣어야 함

			}
			//이곳에 주차 완료조건을 달성하면
			//state = BLACK_STATE_1; 

		}
		else if (state == BLACK_STATE_1){
			I.set_sender_id('B');
			cout << "*************BLACK_STATE_1*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

		


			I.set_packet_id('H');
			send(socket_num, (char*)&I, sizeof(I), 0);


		}



	
	}



	socketMat.socketDisconnect();
	return 0;
}