#include"LaneDetect.h"
using cv::namedWindow;
using cv::Scalar;
using cv::KalmanFilter;
using cv::Mat_;
/*
LaneDetect�� ������.
���� �⺻������ �Ѵ�.
currFrame���� ������ ũ�⸦ �����Ѵ�. (�������� �ޱ����� 640 * 480���� ����.)
�帧
LaneDetect -> nextFrame -> getLane(ROI����) -> markLane(��ó��) -> blobRemoval(��ó�� + Draw) -> getAngle
*/
LaneDetect::LaneDetect(Mat startFrame)
{
	currFrame = Mat(640, 480, CV_8UC1, 0.0);                       //initialised the image size to 640x480
	resize(startFrame, currFrame, currFrame.size());              // resize the input to required size

	temp = Mat(currFrame.rows, currFrame.cols, CV_8UC1, 0.0);     //stores possible lane markings
	temp2 = Mat(currFrame.rows, currFrame.cols, CV_8UC1, 0.0);    //stores finally selected lane marks
	//currFrame�� �̹��� ũ���� temp�� �����س��´�.
	vanishingPt = currFrame.cols / 2;
	//vanishingPt = currFrame.rows / 2;                            //for simplicity right now
	ROIrows = currFrame.rows - vanishingPt;                      //rows in regivon of interest
	minSize = 0.00015 * (currFrame.cols*currFrame.rows);          //min size of any region to be selected as lane
	maxLaneWidth = 0.025 * currFrame.cols;                       //approximate max lane width based on image size 5 0.025 �ν��ϴ� Lane ũ�Ⱑ �޶�����.
	smallLaneArea = 7 * minSize;
	longLane = 0.3 * currFrame.rows;
	ratio = 4;
	//these mark the possible ROI for vertical lane segments and to filter vehicle glare
	//ROI 4 vertical lane segments
	vertical_left = 2 * currFrame.cols / 5;
	vertical_right = 3 * currFrame.cols / 5;
	vertical_top = 2 * currFrame.rows / 3;
	//4���� â ����
	namedWindow("lane", 2);
	namedWindow("midstep", 2);
	namedWindow("currframe", 2);
	namedWindow("laneBlobs", 2);


	getLane();
}
//ROI���� �Լ�
void LaneDetect::getLane()
{

	//ROI = bottom half
	for (int i = vanishingPt; i < currFrame.rows; i++) //vP = 1/2 curr.row. ROI ����.
	for (int j = 0; j < currFrame.cols; j++)
	{
		temp.at<uchar>(i, j) = 0;
		temp2.at<uchar>(i, j) = 0;
		//temp, temp2 ROI ����
	}
	imshow("currframe", currFrame); //No setting image
	blobRemoval();
}
//�������� ��ó�� �˰���.
void LaneDetect::markLane()
{
	for (int i = vanishingPt; i < currFrame.rows; i++) //1/2 row
	{
		// IF COLOUR IMAGE IS GIVEN then additional check can be done
		// lane markings RGB values will be nearly same to each other(i.e without any hue)
		// min lane width is taken to be 5
		// �� ����(RGB value)�� �񱳸� ���Ͽ� Lane����.
		laneWidth = 5 + maxLaneWidth*(i - vanishingPt) / ROIrows;
		for (int j = laneWidth; j < currFrame.cols - laneWidth; j++)
		{
			diffL = currFrame.at<uchar>(i, j) - currFrame.at<uchar>(i, j - laneWidth);
			diffR = currFrame.at<uchar>(i, j) - currFrame.at<uchar>(i, j + laneWidth);
			diff = diffL + diffR - abs(diffL - diffR);
			diffThreshLow = currFrame.at<uchar>(i, j) >> 1;   //1 right bit shifts to make it 0.5 times
			//diffThreshTop = 1.2*currFrame.at<uchar>(i,j);
			//both left and right differences can be made to contribute
			//at least by certain threshold (which is >0 right now)
			//total minimum Diff should be at least more than 5 to avoid noise
			if (diffL > 0 && diffR > 0 && diff > 5)
			if (diff >= diffThreshLow /*&& diff<= diffThreshTop*/) {
				temp.at<uchar>(i, j) = 255;

			}
		}
	}
}
//���������� ������ ���ϴ� �Լ�.
void LaneDetect::setAngle(Point mid_point)
{
	Point mid_xy;
	mid_xy.x = 240;
	mid_xy.y = 640;
	float distance_a = mid_point.x - mid_xy.x;
	float distance_b = mid_xy.y - mid_point.y;
	//get angle
	angle = atan2f(distance_b, distance_a) * 180 / 3.14 - 60;
}

float LaneDetect::getAngle()
{
	return angle;
}
void LaneDetect::blobRemoval()
{
	markLane();
	vector<vector<Point>> temp_sosil;
	// find all contours in the binary image
	// temp���� ���� contour�� ����.
	temp.copyTo(binary_image);
	findContours(binary_image, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	deque<int>circle_que;
	// for removing invalid blobs


	if (!contours.empty())
	{
		for (size_t i = 0; i < contours.size(); ++i)
		{
			//====conditions for removing contours====//
			//contour�� ���� ���ҿ� ���� ����.
			//blob size should not be less than lower threshold
			//minSize = 0.0003 * (currFrame.cols(640)*currFrame.rows(480))   
			//���� col (320) -> ���� col(640) 
			//minsize�� ����� 0.00015�� �ٿ��� �ɵ�.
			contour_area = contourArea(contours[i]);

			if (contour_area > minSize)
			{
				rotated_rect = minAreaRect(contours[i]);
				sz = rotated_rect.size;
				bounding_width = sz.width;
				bounding_length = sz.height;

				//openCV selects length and width based on their orientation
				//so angle needs to be adjusted accordingly
				blob_angle_deg = rotated_rect.angle;
				if (bounding_width < bounding_length)
					blob_angle_deg = 90 + blob_angle_deg;

				//if such big line has been detected then it has to be a (curved or a normal)lane
				//�����̻����� ū lane�� ���͵� lane���� �����Ѵ�.
				if (bounding_length > longLane || bounding_width > longLane)
				{
					drawContours(currFrame, contours, i, Scalar(255), CV_FILLED, 8);
					drawContours(temp2, contours, i, Scalar(255), CV_FILLED, 8);
					for (int q = 0; q < contours.size(); q++) //vector<vector<point>>
					{
						if (contours[q].size() >= 80) {
							circle_que.push_back(q);

						}
					}
				}
				//angle of orientation of blob should not be near horizontal or vertical
				//vertical blobs are allowed only near center-bottom region, where centre lane mark is present
				//length:width >= ratio for valid line segments
				//if area is very small then ratio limits are compensateda
				//�.
				//��� ��ó���� �ٸ��� �ߴ�. (Scalar(130))
				else if ((blob_angle_deg <-10 || blob_angle_deg >-10) &&
					((blob_angle_deg > -70 && blob_angle_deg < 70) ||
					(rotated_rect.center.y > vertical_top &&
					rotated_rect.center.x > vertical_left && rotated_rect.center.x < vertical_right))) //if condition
				{

					if ((bounding_length / bounding_width) >= ratio || (bounding_width / bounding_length) >= ratio
						|| (contour_area < smallLaneArea && ((contour_area / (bounding_width*bounding_length)) > .75) &&
						((bounding_length / bounding_width) >= 2 || (bounding_width / bounding_length) >= 2)))
					{
						drawContours(currFrame, contours, i, Scalar(130), CV_FILLED, 8);
						drawContours(temp2, contours, i, Scalar(130), CV_FILLED, 8);

					}
				}

			}

		}

	}
	//contour vote.
	//circle_que���� ���� ũ�� �̻�(���Ƿ� 65�� ����)�� contour�� ������ �Ͽ���.
	//circle_que_size�� ũ�Ⱑ 2�̻����� ���� ����. ���� circle que���� ������ ����ִµ�
	//������ 2�� �̻��϶��� ��ڴٴ� �ǹ̰� �ȴ�.
	//���� ������ �� ������ ���� ������ ���Ѵ�. 
	if (circle_que.size() >= 2) {
		KalmanFilter KF(4, 2, 0);
		KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
		Mat_<float> measurement(2, 1); measurement.setTo(Scalar(0));

		int temp_que_number = 0;
		int temp_que_size = 0;
		temp_que_size = circle_que.size();
		vector<int> temp_contours_x;
		vector<int> temp_contours_y;

		for (int i = 0; i < temp_que_size; i++)
		{
			temp_que_number = circle_que[0];
			temp_contours_x.push_back(contours[temp_que_number][0].x);
			temp_contours_y.push_back(contours[temp_que_number][0].y);
			circle_que.pop_front();
		}
		//������ �� �� ������ ���� ���Ѵ�.
		if (temp_contours_x[0] <= temp_contours_x[1]) {
			circle_result.x = temp_contours_x[0] + (temp_contours_x[1] - temp_contours_x[0]) / 2;
			circle_result.y = temp_contours_y[0] + (temp_contours_y[1] - temp_contours_y[0]) / 2;
		}
		else {
			circle_result.x = temp_contours_x[1] + (temp_contours_x[0] - temp_contours_x[1]) / 2;
			circle_result.y = temp_contours_y[1] + (temp_contours_y[0] - temp_contours_y[1]) / 2;
		}
		float angle = 0;
		//circle(temp2, circle_result, 5, (0, 0, 255), -1);
		//setAngle(circle_result);

		KF.statePre.at<float>(0) = circle_result.x;
		KF.statePre.at<float>(1) = circle_result.y;
		KF.statePre.at<float>(2) = 0;
		KF.statePre.at<float>(3) = 0;

		setIdentity(KF.measurementMatrix);
		setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
		setIdentity(KF.measurementNoiseCov, Scalar::all(10));
		setIdentity(KF.errorCovPost, Scalar::all(.1));

		Mat prediction = KF.predict();
		Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
		measurement(0) = circle_result.x;
		measurement(1) = circle_result.y;

		Mat estimated = KF.correct(measurement);

		Point statePt(estimated.at<float>(0), estimated.at<float>(1));
		Point measPt(measurement(0), measurement(1));
		circle(temp2, statePt, 15, 135, -1);
		circle(temp2, measPt, 5, 255, -1);
		setAngle(measPt);

	}

	imshow("midstep", currFrame);
	imshow("laneBlobs", temp2);
	//resize�ʿ��ҵ�
	//mshow("lane", cdst);
}



void LaneDetect::nextFrame(Mat &nxt)
{
	//currFrame = nxt;                        //if processing is to be done at original size

	resize(nxt, currFrame, currFrame.size()); //resizing the input image for faster processing
	getLane();
}
