#include"LaneDetect.h"
clock_t start, stop;  //�ð������� ����

// image ���� �Լ� makeFromVid
// main �Լ�

void makeFromVid()
{
	Mat frame;
	VideoCapture cap(0); // open the video file for reading

	if (!cap.isOpened())  // if not success, exit program
		cout << "Cannot open the video file" << endl;

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
										   //���������� frame�� �޴´�. 
	cout << "Input video's Frame per seconds : " << fps << endl;

	cap.read(frame);

	LaneDetect detect(frame);
	//�����ڿ��� �ʱ�ȭ�� �ȴ�.
	while (1)
	{
		bool bSuccess = cap.read(frame); // read a new frame from video
		if (!bSuccess)                   //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		cvtColor(frame, frame, CV_BGR2GRAY);

		start = clock();
		detect.nextFrame(frame);
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
}

int main()
{
	makeFromVid();
	// makeFromVid("/home/yash/opencv-2.4.10/programs/road.m4v");
	waitKey(0);
	destroyAllWindows();
}