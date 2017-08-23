
#include "WinsockMatTransmissionServer.h"


WinsockMatTransmissionServer::WinsockMatTransmissionServer(void)
{
}


WinsockMatTransmissionServer::~WinsockMatTransmissionServer(void)
{
}


int WinsockMatTransmissionServer::socketConnect(int PORT)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);

	int nRecvBuf = 1024 * 1024 * 10;
	setsockopt(sockConn, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
}


void WinsockMatTransmissionServer::socketDisconnect(void)
{
	closesocket(sockConn);
}

int WinsockMatTransmissionServer::receive(cv::Mat& image)
{
	cv::Mat img(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, cv::Scalar(0));

	int needRecv = sizeof(recvbuf);
	int count = 0;

	while (1)
	{
		for (int i = 0; i < 32; i++)
		{
			int pos = 0;
			int len0 = 0;

			while (pos < needRecv)
			{
				len0 = recv(sockConn, (char*)(&data) + pos, needRecv - pos, 0);
				if (len0 < 0)
				{
					printf("Server Recieve Data Failed!\n");
					return -1;
				}
				pos += len0;
			}

			count = count + data.flag;

			int num1 = IMG_HEIGHT / 32 * i;
			for (int j = 0; j < IMG_HEIGHT / 32; j++)
			{
				int num2 = j * IMG_WIDTH * 3;
				uchar* ucdata = img.ptr<uchar>(j + num1);
				for (int k = 0; k < IMG_WIDTH * 3; k++)
				{
					ucdata[k] = data.buf[num2 + k];
				}
			}

			if (data.flag == 2)
			{
				if (count == 33)
				{
					image = img;
					return 1;
					count = 0;
				}
				else
				{
					count = 0;
					i = 0;
				}
			}
		}
	}
}
SOCKET WinsockMatTransmissionServer::get_socket(void){
	return sockConn;
}