#include"LaneDetect.h"
clock_t start, stop;  //시간측정용 변수
clock_t whole_start, whole_stop;
void makeFromVid();


int main()
{
	makeFromVid();
	waitKey(0);
	destroyAllWindows();
}







void makeFromVid()
{
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
	whole_start = clock();
	//생성자에서 초기화가 된다.
	while (1)
	{
		bool bSuccess = cap.read(frame); // read a new frame from video
		if (!bSuccess)                   //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}
		if (whole_stop <=10) {
			temp_angle = detect.getAngle();
		}
		else {
			temp_angle = detect.get_K_Angle();
		}
		cvtColor(frame, frame, CV_BGR2GRAY);
		//circle(detect.temp2, Point(130, 200), 255, 30);
		start = clock();
		detect.nextFrame(frame);
		
		stop = clock();
		whole_stop = clock()/1000;
		cout << "fps : " << 1.0 / (((double)(stop - start)) / CLOCKS_PER_SEC) << endl;
		cout << "angle : " << temp_angle << endl;
		cout << "Clock : " << whole_stop << endl;
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
