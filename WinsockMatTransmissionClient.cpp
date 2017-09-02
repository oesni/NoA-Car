
#include "WinsockMatTransmissionClient.h"


WinsockMatTransmissionClient::WinsockMatTransmissionClient(void)
{
}


WinsockMatTransmissionClient::~WinsockMatTransmissionClient(void)
{
}


int WinsockMatTransmissionClient::socketConnect(const char* IP, int PORT)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}

	err = (sockClient = socket(AF_INET, SOCK_STREAM, 0));
	if (err < 0) {
		printf("create socket error: %s(errno: %d)\n\n", strerror(errno), errno);
		return -1;
	}
	else
	{
		printf("create socket successful!\nnow connect ...\n\n");
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(IP);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);

	err = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err < 0)
	{
		printf("connect error: %s(errno: %d)\n\n", strerror(errno), errno);
		return -1;
	}
	else
	{
		printf("connect successful!\n\n");
		return 1;
	}
}


void WinsockMatTransmissionClient::socketDisconnect(void)
{
	closesocket(sockClient);
	WSACleanup();
}

int WinsockMatTransmissionClient::transmit(cv::Mat image)
{
	if (image.empty())
	{
		printf("empty image\n\n");
		return -1;
	}

	if (image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)
	{
		printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);
		return -1;
	}

	for (int k = 0; k < 32; k++)
	{
		int num1 = IMG_HEIGHT / 32 * k;
		for (int i = 0; i < IMG_HEIGHT / 32; i++)
		{
			int num2 = i * IMG_WIDTH * 3;
			uchar* ucdata = image.ptr<uchar>(i + num1);
			for (int j = 0; j < IMG_WIDTH * 3; j++)
			{
				data.buf[num2 + j] = ucdata[j];
			}
		}

		if (k == 31)
			data.flag = 2;
		else
			data.flag = 1;

		if (send(sockClient, (char *)(&data), sizeof(data), 0) < 0)
		{
			printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
			return -1;
		}
	}
}

SOCKET WinsockMatTransmissionClient::get_SOCKET(void){
	return sockClient;
}