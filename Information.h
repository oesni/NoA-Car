#include <time.h>
#include <iostream>
using namespace std;
class information{
private:
	char sender_id; //1 byte
	char packet_id; //1 byte
	long packet_size; //4 byte
	time_t ltime; // 8 byte
	
public:
	information(){
		sender_id = 0; packet_id = 0; packet_size = 0;
		time(&ltime);

		struct tm* timeinfo = gmtime(&ltime);
		ltime = mktime(timeinfo);
		cout << "Information created\nUNIX time (when created): " << ltime << endl;
		cout << "sizeof ltime : " << sizeof(ltime) << endl;

	};
	~information(){
		cout << endl << "Information exit" << endl;
	};
	void set_sender_id(char val){
		sender_id = val;
	};
	void set_packet_id(char val){
		packet_id = val;
	}


	void set_time(void){
		time(&ltime);
		struct tm* timeinfo = gmtime(&ltime);
		ltime = mktime(timeinfo);
	}

	char get_sender_id(void){
		return sender_id;
	}
	char get_packet_id(void){
		return packet_id;
	}

	time_t get_time(void){
		return ltime;
	}



};