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
#include "LaneDetect.h"
#include <iostream>

using namespace cv;

clock_t start, stop;
char* portName = "\\\\.\\COM6";
SerialPort* arduino;

char sendString[MAX_DATA_LENGTH];

int main()
{
	unsigned char buffer[2], buf_char; int buf_int, _a, _b;
	buf_int = 1667;
	int state = AMB_STATE_1;
	int socket_num;
	int bytesSent;
	information I;
	
	arduino = new SerialPort(portName);
	if (arduino->isConnected()){
		cout << "ok arduino connection has done succesfully~" << endl << endl;
	}
	else{
		cout << "oh no arduino connection has failed" << endl << endl;
	}

	WinsockMatTransmissionClient socketMat;
	if (socketMat.socketConnect("165.246.235.33", 23957) < 0)
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
			//1) 아무것도 없는 default 상태 모든게 정지해있음
			//여기선 서버로부터 신호를 대기중임
			//	신호를 수신하면 2) 로 넘어감


			I.set_sender_id('A');
			cout << "*************AMB_STATE_0*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

			

			recv(socket_num, (char*)&I, sizeof(I), 0);

			if (I.get_packet_id() == 'A'){
				state = AMB_STATE_1;
			}
		}
		else if (state == AMB_STATE_1){
			//2) 사고 지역을 향해 주행하는 상태
			//	여기선 계속하여 번호판 인식(방해차량)을 하고 인식을 했을때 방해차량 번호판과 같다면
			//	비키라는 신호를 서버에게 보내고 차량 정지 후 서버로부터 출발 신호를 대기함
			//	출발 신호를 수신하면 3)으로 넘어감
			I.set_sender_id('A');
			cout << "*************AMB_STATE_1*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;




			Mat frame;
			VideoCapture cap(0); // open the video file for reading
			float temp_angle = 0;
			if (!cap.isOpened())  // if not success, exit program
				cout << "Cannot open the video file" << endl;

			//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

			double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
			//원본영상의 frame을 받는다. 
			cout << "Input video's Frame per seconds : " << fps << endl;

			cap.read(frame);

			LaneDetect detect(frame);
			//차선인식 설정 끝
			while (1){
				bool bSuccess = cap.read(frame); // read a new frame from video
				if (!bSuccess)                   //if not success, break loop
				{
					cout << "Cannot read the frame from video file" << endl;
					break;
				}
				cvtColor(frame, frame, CV_BGR2GRAY);

				start = clock();
				detect.nextFrame(frame);
				//		img = detect.currFrame;
				temp_angle = detect.getAngle();
				//img = detect.temp2;
				stop = clock();
				cout << "fps : " << 1.0 / (((double)(stop - start)) / CLOCKS_PER_SEC) << endl;
				buf_int = (int)temp_angle * (20000 / 360);
				if (buf_int >= 3333) buf_int = 3333;
				else if (buf_int <= 0) buf_int = 1;
				cout << "angle_pulse : " << buf_int << endl;
				_a = buf_int % 256; buffer[0] = (char)_a;
				_b = (buf_int / 256); buffer[1] = (char)_b;
				


				stop = clock();

				if (arduino->isConnected()){
					bool hasWritten = arduino->writeSerialPort(&buffer[1], MAX_DATA_LENGTH);
					if (hasWritten) std::cout << "Data Written Successfully" << std::endl;
					else std::cout << "Data was not written" << std::endl;
				}

				cout << "fps : " << 1.0 / (((double)(stop - start)) / CLOCKS_PER_SEC) << endl;
				if (waitKey(10) == 27) //wait for 'esc' key press for 10 ms. If 'esc' key is pressed, break loop
				{
					cout << "video paused!, press q to quit, any other key to continue" << endl;
					if (waitKey(0) == 'q')
					{
						cout << "terminated by user" << endl;
						break;
					}
				}
			}



			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			//여기에 방해차량 번호판을 인식하는 코드가 필요함
			//주행코드도 필요함
			//
			//
			//if(방해차량 번호판 옳다면) {
			//	I.set_packet_id('D');
			//	send(socket_num, (char*)&I, sizeof(I), 0)
			//  그리고 차량 정지
				//recv(socket_num, (char*)&I, sizeof(I), 0);
				//if (I.get_packet_id() == 'C'){
				//	state = AMB_STATE_2;
				//}
			//}
			
		}
		else if (state == AMB_STATE_2){
			//3) 사고 지역을 향해 주행하는 상태(앞에 방해차량은 제거됨)
			//	여기선 계속하여 번호판 인식(사고차량)을 하고 인식을 했을때 사고차량 번호판과 같다면
			//	4) 로 넘어감
			I.set_sender_id('A');
			cout << "*************AMB_STATE_2*************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;



			Mat frame;
			VideoCapture cap(0); // open the video file for reading
			float temp_angle = 0;
			if (!cap.isOpened())  // if not success, exit program
				cout << "Cannot open the video file" << endl;

			//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

			double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
			//원본영상의 frame을 받는다. 
			cout << "Input video's Frame per seconds : " << fps << endl;

			cap.read(frame);

			LaneDetect detect(frame);
			//차선인식 설정 끝
			while (1){
				bool bSuccess = cap.read(frame); // read a new frame from video
				if (!bSuccess)                   //if not success, break loop
				{
					cout << "Cannot read the frame from video file" << endl;
					break;
				}
				cvtColor(frame, frame, CV_BGR2GRAY);

				start = clock();
				detect.nextFrame(frame);
				//		img = detect.currFrame;
				temp_angle = detect.getAngle();
				//img = detect.temp2;
				stop = clock();
				cout << "fps : " << 1.0 / (((double)(stop - start)) / CLOCKS_PER_SEC) << endl;
				buf_int = (int)temp_angle * (20000 / 360);
				if (buf_int >= 3333) buf_int = 3333;
				else if (buf_int <= 0) buf_int = 1;
				cout << "angle_pulse : " << buf_int << endl;
				_a = buf_int % 256; buffer[0] = (char)_a;
				_b = (buf_int / 256); buffer[1] = (char)_b;



				stop = clock();
				cout << "fps : " << 1.0 / (((double)(stop - start)) / CLOCKS_PER_SEC) << endl;
				if (waitKey(10) == 27) //wait for 'esc' key press for 10 ms. If 'esc' key is pressed, break loop
				{
					cout << "video paused!, press q to quit, any other key to continue" << endl;
					if (waitKey(0) == 'q')
					{
						cout << "terminated by user" << endl;
						break;
					}
				}
			}

			//여기에 사고차량 번호판을 인식하는 코드가 필요함
			//주행 코드도 필요함
			//if(사고차량 번호판 옳다면) {
			//	I.set_packet_id('F');
			//	send(socket_num, (char*)&I, sizeof(I), 0)
			//  state == AMB_STATE_3
			//}
			

		}
		else if (state == AMB_STATE_3){
			//4) 사고차량 번호판을 확인한 상태
			//	여기선 서버로 영상신호를 보냄
			//	계속하여 영상 신호를 보내다가 일정 거리가 되면 정지하고 도착신호를 서버에 전송하고 종료
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
			//1) 아무것도 없는 default 상태 서버와 Amb 는 작동중임
			//	여기선 server 로 부터 비키라는 신호를 수신함
			//	신호를 수신하면 2) state 로 넘어감
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
			//2) 여기선 갓길로 주차를 하는 주행 구현해야함
			//	주차가 완료 되면 서버에게 신호를 보내고 종료

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