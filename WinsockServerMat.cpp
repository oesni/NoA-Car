#include "WinsockMatTransmissionServer.h"
#include "Information.h"


#define SERVER_STATE_0 0
#define SERVER_STATE_1 1
#define SERVER_STATE_2 2
#define SERVER_STATE_3 3
#define SERVER_STATE_4 4


int main()
{
	int state = SERVER_STATE_4;
	int socket_num;
	int i = 0;
	cv::Mat image;
	information I;
	I.set_sender_id('S');

	WinsockMatTransmissionServer socketMat;
	if (socketMat.socketConnect(23957) < 0)
	{
		cout << "AMB_connection error" << endl << endl;
		return 0;
	}
	socket_num = (int)socketMat.get_socket();
	
	for (;;)
	{
		I.set_time();

		if (state == SERVER_STATE_0){
			//*******************************************************************
			//*******************************************************************
			//*******************************************************************
			//**********
			//**********	아무것도 없는 dafault 상태 모든게 정지해있음
			//				여기선 상황발생의 조건이 필요함
			//				상황발생하면 Amb 로 신호 쏴주고 2) 로 넘어감
			//**********
			//**********
			//**********
			//**********
			//*******************************************************************
			//*******************************************************************
			//*******************************************************************
	
			cout << "************SERVER_STATE_0****************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

			

			I.set_packet_id('A');
			send(socket_num, (char*)&I, sizeof(I), 0);
			state = SERVER_STATE_1;
		}






		else if (state == SERVER_STATE_1){

			cout << "************SERVER_STATE_1****************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

	
			recv(socket_num, (char*)&I, sizeof(I), 0);
			if (I.get_packet_id() == 'D'){
				state = SERVER_STATE_2;
			}
		}






		else if (state == SERVER_STATE_2){
			cout << "************SERVER_STATE_2****************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

	
			


			I.set_packet_id('B');

			send(socket_num, (char*)&I, sizeof(I), 0);

			state = SERVER_STATE_3;
		}










		else if (state == SERVER_STATE_3){
			cout << "************SERVER_STATE_3****************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

			
		
			
			recv(socket_num, (char*)&I, sizeof(I), 0);
			//주의 여기서 socket 은 방해차량과 통신하고 있는 socket 이여야 함! 수정 해야함!!
			if (I.get_packet_id() == 'H'){
				state = SERVER_STATE_4;
				I.set_packet_id('C');
				send(socket_num, (char*)&I, sizeof(I), 0);
			}
		}












		else if (state == SERVER_STATE_4){
			cout << "************SERVER_STATE_4****************" << endl;
			cout << "sender_id : " << I.get_sender_id() << endl;
			cout << "time : " << I.get_time() << endl;
			cout << "packet_id : " << I.get_packet_id() << endl << endl;

			
			

			if (socketMat.receive(image) > 0)
			{
				cvtColor(image, image, CV_BGR2GRAY);
				cv::imshow("get_from_client", image);
				cv::waitKey(27);
				
			}

			recv(socket_num, (char*)&I, sizeof(I), 0);
			if (I.get_packet_id() == 'G'){
				cout << "All situation end..." << endl << endl;
				break;
			}


		}










		
		
	}


	cout << "well done all program have been closed. so long sucker~" << endl << endl;

	socketMat.socketDisconnect();
	return 0;
}