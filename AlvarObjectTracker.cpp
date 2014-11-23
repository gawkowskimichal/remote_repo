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
	cout << "AlvarObjectTracker tracker"<< endl;
	config = &conf;
	marker_size = atof(config->getValueByKey("markerSize").c_str());
	double width = atof(config->getValueByKey("resolution_y").c_str());
	double height = atof(config->getValueByKey("resolution_x").c_str());
	cout << "readCameraParams"<< endl;
	bool data_read = readCameraParams(calibfn, cameraMatrix, distCoeffs);
	if (!data_read){
		cout << "Calib file didn't read!"<< endl;
	}
	//cam.SetCalib(calibfn.c_str(), width, height);
}


bool AlvarObjectTracker::readCameraParams( string calib_file, Mat& cameraMatrix, Mat& distCoeffs ) {
    cv::FileStorage fs;
    cout << "fs" << endl;
    fs.open( calib_file, FileStorage::READ );
    cout << "fs open" << endl;
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
	cout << "track0" << endl;
	IplImage image = picture;
	IplImage *image_tmp = &image;
	String result;
	cout << "track1" << endl;
	bool flip_image = (image_tmp->origin?true:false);
	if (flip_image) {
		cvFlip(image_tmp);
		image_tmp->origin = !image_tmp->origin;
	}
	cout << "track2" << endl;
	static alvar::MarkerDetector<alvar::MarkerData> marker_detector;
	cout << "track3" << endl;
	marker_detector.SetMarkerSize(marker_size);
	marker_detector.Detect(image_tmp, &cam, true, true);
	cout << "track4" << endl;
	if(marker_detector.markers->size() > 0){
		cout << "Found sth" << endl;
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
		cout << "try undistort" << endl;
		undistortPoints(orgPoint, udorgPoint, cameraMatrix, distCoeffs);
		Mat imageUndistortedMat;
		undistort(picture, imageUndistortedMat, cameraMatrix, distCoeffs);
		vector<Point2f> pointBuf;
		Mat homography = findHomography(imageUndistortedMat, Size(6,4), float(100), cameraMatrix, distCoeffs, pointBuf);
		cout << "try perspectiveTransform" << endl;
		perspectiveTransform( udorgPoint, rorgPoint, homography);
		cout << "try output" << endl;
		for (int i = 0; i < 3; ++i) {
			result += boost::lexical_cast<std::string>(i) + " " +
						boost::lexical_cast<std::string>(udorgPoint[i].x) + " " +
						boost::lexical_cast<std::string>(udorgPoint[i].y) + " " +
						time + "\n";
		}
		result += "\n";
	}
	cout << "track5" << endl;
	return result;
};

vector<String> AlvarObjectTracker::trackInPictures(vector<std::pair<Mat,String>> pictures){
	vector<String> result;
	String partial;
	for (std::vector<std::pair<Mat,String>>::iterator it = pictures.begin(); it != pictures.end(); ++it){
		cout << "Tracking in picture" << endl;
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

Mat AlvarObjectTracker::findHomography(Mat img, Size pattern_size, float squareSize,
                    Mat intrinsic_matrix, Mat distortion_coeffs, vector<Point2f> &pointBuf)
{
    bool found;
    found = findChessboardCorners( img, pattern_size, pointBuf,
                                   CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
    vector<Point2f> objectPoints;
    Mat homography;
    if(pointBuf.size() > 0 && pointBuf.size() == objectPoints.size())
    {
        vector<Point2f> cpointBuf(pointBuf);
        undistortPoints(pointBuf, cpointBuf, intrinsic_matrix, distortion_coeffs);//TODO spr
        homography = cv::findHomography(cpointBuf, objectPoints, homography);
        //cout << "homography success" << endl;
    }
    return homography;
}
