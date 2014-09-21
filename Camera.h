/*
 * Camera.h
 *
 *  Created on: 21 Sep 2014
 *      Author: mgw
 */

#ifndef CAMERA_H_
#define CAMERA_H_
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Camera {
public:
	int index;
	Mat frame,frameCopy,image;
	CvCapture *cam;
	Camera(int i);
	void setCaptureOption(int option, double value);
	virtual ~Camera();
};

#endif /* CAMERA_H_ */
