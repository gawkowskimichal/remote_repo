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
#include <stdlib.h>
#include "Configuration.h"
#include "Camera.h"
#include <boost/lexical_cast.hpp>


#ifndef VIDEOCONNECTOR_H_
#define VIDEOCONNECTOR_H_

using namespace cv;
using namespace std;

class VideoConnector {
public:
	int CameraCounter;
	std::vector<Camera*> cameras;
	CvCapture *capture1, *capture2;
	Mat frame1, frameCopy1, image1, frame2, frameCopy2, image2;
	VideoConnector();
	virtual ~VideoConnector();
	int captureVideo(int i);
	int captureMultipleVideo();
	Mat captureSnapshot(int i);
	vector<Mat> captureMultipleSnapshot();
	void saveImageToFile(Mat image, String path);
	Mat readImageFromFile(String path);
	void initCameras(Configuration conf);
	void setCaptureOptions(Configuration conf);
	int resetCameras();
	void shutdownCameras();
	int getCameraCount(Configuration conf);
	void getCalibrationMaterial(Configuration conf, int i);
	void getCalibrationMultipleMaterial(Configuration conf);
};

#endif /* VIDEOCONNECTOR_H_ */
