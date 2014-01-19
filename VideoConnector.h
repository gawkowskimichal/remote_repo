/*
 * VideoConnector.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>


#ifndef VIDEOCONNECTOR_H_
#define VIDEOCONNECTOR_H_

using namespace cv;
using namespace std;

class VideoConnector {
public:
	CvCapture* capture;
	Mat frame, frameCopy, image;
	VideoConnector();
	virtual ~VideoConnector();
	int captureVideo();
};

#endif /* VIDEOCONNECTOR_H_ */
