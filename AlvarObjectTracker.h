/*
 * AlvarObjectTracker.h
 *
 *  Created on: 5 Oct 2014
 *      Author: mgw
 */


#ifndef ALVAROBJECTTRACKER_H_
#define ALVAROBJECTTRACKER_H_
#include "ObjectTracker.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "MarkerDetector.h"
#include "Configuration.h"
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>

using namespace cv;
using namespace std;
namespace TracingFramework {
class AlvarObjectTracker : public ObjectTracker {
public:
	Configuration *config;
	Mat homographyInternal;
	alvar::Camera cam;
	double marker_size;
	Mat cameraMatrix, distCoeffs;
	bool homographyFound;
	AlvarObjectTracker(Configuration conf, String calibfn);
	virtual ~AlvarObjectTracker();
	String trackInPicture(Mat picture, String time);
	vector<std::pair<Point2f,String>> trackInPictureV(Mat picture, String time);
	vector<Point2f> trackInPicturePixels(Mat picture);
	vector<String> trackInPictures(vector<std::pair<Mat,String>> pictures);
	vector<String> trackInVideo(String filename);
	vector<vector<std::pair<Point2f,String>>> trackInPicturesV(vector<std::pair<Mat,String>> pictures);
	void saveTrackToFile(vector<String> pos, String filename);
	bool readCameraParams( String calib_file, Mat& cameraMatrix, Mat& distCoeffs );
	Mat findHomography(Mat img, Size pattern_size, float squareSize,
            Mat intrinsic_matrix, Mat distortion_coeffs, vector<Point2f> &pointBuf);
	void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point2f>& corners);
};
}
#endif /* ALVAROBJECTTRACKER_H_ */
