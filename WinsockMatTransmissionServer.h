#ifndef __WINSOCKMATTRANSMISSIONSEVER_H__
#define __WINSOCKMATTRANSMISSIONSEVER_H__

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

struct recvbuf
{
	char buf[BUFFER_SIZE];
	int flag;
};

class WinsockMatTransmissionServer
{
public:
	WinsockMatTransmissionServer(void);
	~WinsockMatTransmissionServer(void);

private:
	SOCKET sockConn;
	struct recvbuf data;

public:

	int socketConnect(int PORT);
	int receive(cv::Mat& image);
	void socketDisconnect(void);
	SOCKET get_socket(void);
};

#endif
