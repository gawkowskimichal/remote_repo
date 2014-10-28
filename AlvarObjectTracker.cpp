/*
 * AlvarObjectTracker.cpp
 *
 *  Created on: 5 Oct 2014
 *      Author: mgw
 */

#include "AlvarObjectTracker.h"
#include "Camera.h"

using namespace cv;
using namespace alvar;
using namespace TracingFramework;

AlvarObjectTracker::AlvarObjectTracker(Configuration conf, String calibfn) {
	config = &conf;
	marker_size = atof(config->getValueByKey("markerSize").c_str());
	double width = atof(config->getValueByKey("resolution_y").c_str());
	double height = atof(config->getValueByKey("resolution_x").c_str());
	cam.SetCalib(calibfn.c_str(), width, height);
	readCameraParams(calibfn, cameraMatrix, distCoeffs);
}


bool AlvarObjectTracker::readCameraParams( string calib_file, Mat& cameraMatrix, Mat& distCoeffs ) {
    cv::FileStorage fs( calib_file, FileStorage::READ );
    bool fsIsOpened = fs.isOpened();
    if(fsIsOpened)
    {
        fs["Camera_Matrix"] >> cameraMatrix;
        fs["Distortion_Coefficients"] >> distCoeffs;
    }
    return fsIsOpened;
}


AlvarObjectTracker::~AlvarObjectTracker() {
	// TODO Auto-generated destructor stub
}

String AlvarObjectTracker::trackInPicture(Mat picture, String time){
	IplImage image = picture;
	IplImage *image_tmp = &image;
	String result;
	bool flip_image = (image_tmp->origin?true:false);
	if (flip_image) {
		cvFlip(image_tmp);
		image_tmp->origin = !image_tmp->origin;
	}
	static alvar::MarkerDetector<alvar::MarkerData> marker_detector;
	marker_detector.SetMarkerSize(marker_size);
	marker_detector.Detect(image_tmp, &cam, true, true);
	if(marker_detector.markers->size() > 0){
		int markerIndx = 0;
		int cornerIndx = 0;
		float markerPosX0 = (*(marker_detector.markers))[markerIndx].marker_corners_img[cornerIndx].x;
		float markerPosY0 = (*(marker_detector.markers))[markerIndx].marker_corners_img[cornerIndx].y;
		cornerIndx = 1;
		float markerPosXX = (*(marker_detector.markers))[markerIndx].marker_corners_img[cornerIndx].x;
		float markerPosYX = (*(marker_detector.markers))[markerIndx].marker_corners_img[cornerIndx].y;
		cornerIndx = 2;
		float markerPosXY = (*(marker_detector.markers))[markerIndx].marker_corners_img[cornerIndx].x;
		float markerPosYY = (*(marker_detector.markers))[markerIndx].marker_corners_img[cornerIndx].y;
		vector<Point2f> orgPoint;
		orgPoint.push_back(Point2f(markerPosX0, markerPosY0));
		orgPoint.push_back(Point2f(markerPosXX, markerPosYX));
		orgPoint.push_back(Point2f(markerPosXY, markerPosYY));
		vector<Point2f> udorgPoint(orgPoint);
		vector<Point2f> rorgPoint(orgPoint);
		undistortPoints(orgPoint, udorgPoint, cameraMatrix, distCoeffs);
		Mat homography;
		homography = findHomography(orgPoint, udorgPoint, homography);
		perspectiveTransform( udorgPoint, rorgPoint, homography);
		for (int i = 0; i < 2; ++i) {
			result += boost::lexical_cast<std::string>(rorgPoint[i].x) + " " + boost::lexical_cast<std::string>(rorgPoint[i].y) + " " + time;
		}
	}
	return result;
};

vector<String> AlvarObjectTracker::trackInPictures(vector<std::pair<Mat,String>> pictures){
	vector<String> result;
	String partial;
	for (std::vector<std::pair<Mat,String>>::iterator it = pictures.begin(); it != pictures.end(); ++it){
		partial = trackInPicture((*it).first,(*it).second);
		result.push_back(partial);
	}
	return result;
};

vector<String> AlvarObjectTracker::trackInVideo(String filename){
	vector<String> result;
	return result;
};

void AlvarObjectTracker::saveTrackToFile(vector<String> pos, String filename){

};
