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
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "Configuration.h"
#include "Camera.h"
#include <time.h>
#include <ctime>
#include <boost/lexical_cast.hpp>
#include <boost/thread/condition_variable.hpp>


#ifndef VIDEOCONNECTOR_H_
#define VIDEOCONNECTOR_H_

using namespace cv;
using namespace std;
namespace TracingFramework {
class VideoConnector {
public:
	int CameraCounter;
	std::vector<Camera*> cameras;
	CvCapture *capture1, *capture2;
	Mat frame1, frameCopy1, image1, frame2, frameCopy2, image2;
	int internalCounter;
	vector<Mat> CamerasMatrices;
	vector<Mat> DistCoeffs;
	boost::condition_variable images_to_write;
	boost::mutex file_mutex;
	VideoConnector();
	virtual ~VideoConnector();
	int captureVideo(int i);
	int captureMultipleVideo();
	vector<std::pair<Mat,String>> captureVideoToVector();
	vector<vector<std::pair<Mat,String>>> captureMultipleVideoToVectors();
	int captureVideoToFilesWithInfo(Configuration conf);
	int captureMultipleVideoToFilesWithInfo(Configuration conf);
	void captureVideoToFile(int i, Configuration conf);
	void captureMultipleVideoToFiles(Configuration conf);
	Mat captureSnapshot(int i);
	vector<Mat> captureMultipleSnapshot();
	void saveImageToFile(Mat image, String path);
	Mat readImageFromFile(String path);
	void saveInfoToFile(String info, String path);
	void initCameras(Configuration conf);
	void initCameraConfigs(Configuration conf);
	void setCaptureOptions(Configuration conf);
	int resetCameras();
	void shutdownCameras();
	int getCameraCount(Configuration conf);
	void getCalibrationMaterial(Configuration conf, int i);
	void getCalibrationMultipleMaterial(Configuration conf);
	string getTime();
};
}
#endif /* VIDEOCONNECTOR_H_ */
