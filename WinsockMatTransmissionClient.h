#ifndef __WINSOCKMATTRANSMISSIONCLIENT_H__
#define __WINSOCKMATTRANSMISSIONCLIENT_H__
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <Winsock2.h>

#pragma comment(lib,"WS2_32.lib")


#define IMG_WIDTH 640	
#define IMG_HEIGHT 480	
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/32
struct sentbuf
{
	char buf[BUFFER_SIZE];
	int flag;
};

class WinsockMatTransmissionClient
{
public:
	WinsockMatTransmissionClient(void);
	~WinsockMatTransmissionClient(void);

private:
	SOCKET sockClient;
	struct sentbuf data;
public:
	int socketConnect(const char* IP, int PORT);
	int transmit(cv::Mat image);
	SOCKET get_SOCKET(void);
	void socketDisconnect(void);
};

#endif