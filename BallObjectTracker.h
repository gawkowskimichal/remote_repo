/*
 * BallObjectTracker.h
 *
 *  Created on: 12 Oct 2014
 *      Author: mgw
 */

#ifndef BALLOBJECTTRACKER_H_
#define BALLOBJECTTRACKER_H_
#include "ObjectTracker.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "Configuration.h"
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>

using namespace cv;
using namespace std;


namespace TracingFramework {

class BallObjectTracker : public ObjectTracker {
public:
	Configuration *config;
	String calibrationFile;
	double marker_size;
	Mat cameraMatrix, distCoeffs;
	BallObjectTracker(Configuration conf, String calibfn);
	virtual ~BallObjectTracker();
	String trackInPicture(Mat picture, String time);
	vector<std::pair<Point2f,String>> trackInPictureV(Mat picture, String time);
	vector<Point2f> trackInPicturePixels(Mat picture);
	vector<vector<std::pair<Point2f,String>>> trackInPicturesV(vector<std::pair<Mat,String>> pictures);
	vector<String> trackInPictures(vector<std::pair<Mat,String>> pictures);
	vector<String> trackInVideo(String filename);
	void saveTrackToFile(vector<String> pos, String filename);
	bool readCameraParams( String calib_file, Mat& cameraMatrix, Mat& distCoeffs );
};

} /* namespace TracingFramework */

#endif /* BALLOBJECTTRACKER_H_ */
