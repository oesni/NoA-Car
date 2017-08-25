#include"LaneDetect.h"
#include "opencv2/video/tracking.hpp"

clock_t start, stop;  //시간측정용 변수
void makeFromVid();
#define drawCross( center, color, d )                                 \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )



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
