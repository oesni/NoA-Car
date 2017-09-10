#include"LaneDetect.h"
#include "opencv2/video/tracking.hpp"
#include "SerialComm.h"
using cv::VideoCapture;
using cv::destroyAllWindows;
#define _AMB 1
#define _Kid_car 2

#define _Car_Select _AMB


clock_t start, stop, w_time;  //시간측정용 변수
void makeFromVid();
#define drawCross( center, color, d )                                 \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )



int main()
{
	//통신 최초 설정
	unsigned char buffer[2], buf_char; int buf_int, _a, _b;
	buf_int = 1667;
	CSerialComm serialComm; //SerialComm 객체 생성
	CSerialComm bluetoothmod;
	if (!serialComm.connect("COM7")) {
		cout << "Arduino 보드 연결 실패" << endl; //return -1;
	}
	else
		cout << "Arduino 보드 연결 성공" << endl;
	_a = buf_int % 256; buffer[0] = (char)_a;
	_b = (buf_int / 256); buffer[1] = (char)_b;
	if (!serialComm.sendCommand(buffer[1]))
		cout << "send command failed" << endl;
	else
		cout << "send Command success" << endl;
	if (!serialComm.sendCommand(buffer[0]))
		cout << "send command failed" << endl;
	else
		cout << "send Command success" << endl;
	if (!serialComm.sendCommand('s'))
		cout << "send command failed" << endl;
	else
		cout << "send Command success" << endl;
	if (!bluetoothmod.connect("COM8")) {
		cout << "JMOD-128-1 연결 실패" << endl; //return -1;
	}
	else
		cout << "JMOD-128-1 연결 성공" << endl;
	char test; char testrx;

	while (1) {
		cin >> buf_char;
		if (buf_char == 'o') break;
	}
	//통신 설정 끝
	//차선인식 설정
	
	Mat frame;
	VideoCapture cap(0); // open the video file for reading
	float temp_angle = 0;
	if (!cap.isOpened())  // if not success, exit program
		cout << "Cannot open the video file" << endl;

	cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
										   //원본영상의 frame을 받는다. 
	cout << "Input video's Frame per seconds : " << fps << endl;

	cap.read(frame);

	LaneDetect detect(frame);
	
	//차선인식 설정 끝
	while (1) {
		w_time = clock() / 1000;
		bool bSuccess = cap.read(frame); // read a new frame from video
		if (!bSuccess)                   //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}
		cvtColor(frame, frame, CV_BGR2GRAY);

		start = clock();
		detect.nextFrame(frame);
		//img = detect.currFrame;
		if (w_time < 6) {
			temp_angle = detect.getAngle();
		}
		else {
			temp_angle = detect.get_K_Angle();
		}
		//float temp_angle; cin >> temp_angle;
		//temp_angle = detect.getAngle();
		//img = detect.temp2;
		stop = clock();
		cout << "fps : " << 1.0 / (((double)(stop - start)) / CLOCKS_PER_SEC) << endl;
		//buf_int = (int)temp_angle * (20000 / 360);
		if (temp_angle >= 60) temp_angle = 60;
		else if (temp_angle <= 0) temp_angle = 1;
		buf_int = (int)temp_angle * (20000 / 360);
		if (buf_int >= 3000) buf_int = 3000;
		else if (buf_int <= 200) buf_int = 200;
		cout << "angle_pulse : " << buf_int << endl;
		cout << "실제 보는 각도 : " << temp_angle << endl;
		_a = buf_int % 256; buffer[0] = (char)_a;
		_b = (buf_int / 256); buffer[1] = (char)_b;
		if (!serialComm.sendCommand(buffer[1]))
			cout << "send command failed" << endl;
		else
			cout << "send Command success" << endl;
		if (!serialComm.sendCommand(buffer[0]))
			cout << "send command failed" << endl;
		else
			cout << "send Command success" << endl;
		if (!serialComm.sendCommand('g'))
			cout << "send command failed" << endl;
		else
			cout << "send Command success" << endl;
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
	//makevid()
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
	//Point temp_p = detect.circle_result;
	//생성자에서 초기화가 된다.
	/*KalmanFilter KF(4, 2, 0);
	KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
	Mat_<float> measurement(2, 1); measurement.setTo(Scalar(0));
	KF.statePre.at<float>(0) = temp_p.x;
	KF.statePre.at<float>(1) = temp_p.y;
	KF.statePre.at<float>(2) = 0;
	KF.statePre.at<float>(3) = 0;
	setIdentity(KF.measurementMatrix);
	setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
	setIdentity(KF.measurementNoiseCov, Scalar::all(10));
	setIdentity(KF.errorCovPost, Scalar::all(.1));
	vector<Point> mousev, kalmanv;
	mousev.clear();
	kalmanv.clear();
	Mat img(640, 480, CV_8UC3,0.0);
	*/
	while (1)
	{

		//Mat prediction = KF.predict();
		//Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
		//temp_p = detect.circle_result;
		//// Get mouse point
		//
		//measurement(0) = temp_p.x;
		//measurement(1) = temp_p.y;

		//Mat estimated = KF.correct(measurement);

		//Point statePt(estimated.at<float>(0), estimated.at<float>(1));
		//Point measPt(measurement(0), measurement(1));
		//// plot points

		//
		//mousev.push_back(measPt);
		//kalmanv.push_back(statePt);
		////drawCross(statePt, Scalar(255, 255, 255), 3);
		////drawCross(measPt, Scalar(0, 0, 255), 3);
		////statePt, measPt
		//for (int i = 0; i < mousev.size() - 1; i++)
		//	line(img, mousev[i], mousev[i + 1], Scalar(255, 255, 0), 1);

		//for (int i = 0; i < kalmanv.size() - 1; i++)
		//	line(img, kalmanv[i], kalmanv[i + 1], Scalar(0, 155, 255), 1);


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
		cout << "angle : " << temp_angle << endl;
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
