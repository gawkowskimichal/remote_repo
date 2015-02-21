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
	homographyFound = false;
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
	cout << "track2" << endl;
	Mat imageUndistortedMat;
	vector<Point2f> pointBuf;
	undistort(picture, imageUndistortedMat, cameraMatrix, distCoeffs);
	Mat homography;
	if (!this->homographyFound){
		homography = findHomography(imageUndistortedMat, Size(9,6), float(2.5), cameraMatrix, distCoeffs, pointBuf);
		homographyInternal = homography.clone();
	} else {
		homography = homographyInternal.clone();
	}
	static alvar::MarkerDetector<alvar::MarkerData> marker_detector;
	cout << "track3" << endl;
	marker_detector.SetMarkerSize(marker_size);
	marker_detector.Detect(image_tmp, &cam, true, true);
	cout << "track4" << endl;
	cout << "Markers: " << marker_detector.markers->size() << endl;
	if(homography.data != NULL && marker_detector.markers->size() > 0){
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
		cout << "try undistort" << endl;
		undistortPoints(orgPoint, udorgPoint, cameraMatrix, distCoeffs);
		vector<Point2f> rorgPoint(udorgPoint);
		cout << "try perspectiveTransform" << endl;
		perspectiveTransform( udorgPoint, rorgPoint, homography);
		cout << "try output" << endl;
		for (int i = 0; i < 3; ++i) {
			result += boost::lexical_cast<std::string>(i) + " " +
						boost::lexical_cast<std::string>(rorgPoint[i].x) + " " +
						boost::lexical_cast<std::string>(rorgPoint[i].y) + " " +
						time + "\n";
		}
		result += "\n";
	}
	cout << "track5" << endl;
	return result;
};


vector<std::pair<Point2f,String>> AlvarObjectTracker::trackInPictureV(Mat picture, String time){
	cout << "track0" << endl;
	IplImage image = picture;
	IplImage *image_tmp = &image;
	vector<std::pair<Point2f,String>> result;
	cout << "track2" << endl;
	Mat imageUndistortedMat;
	vector<Point2f> pointBuf;
	undistort(picture, imageUndistortedMat, cameraMatrix, distCoeffs);
	Mat homography;
	if (!this->homographyFound){
		homography = findHomography(imageUndistortedMat, Size(9,6), float(2.5), cameraMatrix, distCoeffs, pointBuf);
		homographyInternal = homography.clone();
	} else {
		homography = homographyInternal.clone();
	}
	static alvar::MarkerDetector<alvar::MarkerData> marker_detector;
	cout << "track3" << endl;
	marker_detector.SetMarkerSize(marker_size);
	marker_detector.Detect(image_tmp, &cam, true, true);
	cout << "track4" << endl;
	cout << "Markers: " << marker_detector.markers->size() << endl;
	if(homography.data != NULL && marker_detector.markers->size() > 0){
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
		cout << "try undistort" << endl;
		undistortPoints(orgPoint, udorgPoint, cameraMatrix, distCoeffs);
		vector<Point2f> rorgPoint(udorgPoint);
		cout << "try perspectiveTransform" << endl;
		perspectiveTransform( udorgPoint, rorgPoint, homography);
		cout << "try output" << endl;
		Point2f partial;
		for (int i = 0; i < 3; ++i) {
			partial.x = rorgPoint[i].x;
			partial.y = rorgPoint[i].y;
			result.push_back(std::make_pair(partial,time));
		}
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

vector<vector<std::pair<Point2f,String>>>  AlvarObjectTracker::trackInPicturesV(vector<std::pair<Mat,String>> pictures){
	vector<vector<std::pair<Point2f,String>>> result;
	vector<std::pair<Point2f,String>> partial;
	for (std::vector<std::pair<Mat,String>>::iterator it = pictures.begin(); it != pictures.end(); ++it){
		cout << "Tracking in picture" << endl;
		partial = trackInPictureV((*it).first,(*it).second);
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
                    Mat intrinsic_matrix, Mat distortion_coeffs, vector<Point2f> &pointBuf){
    bool found = false;
    flip( img, img, 0 );
	found = findChessboardCorners( img, pattern_size, pointBuf,
                                   CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
	if (found){
		cout << "findChessboardCorners found sth" << endl;
	} else {
		cout << "findChessboardCorners didn't find anth" << endl;
	}
    vector<Point2f> objectPoints;
    calcBoardCornerPositions(pattern_size, squareSize, objectPoints);
    Mat homography;
    cout << "pointBuf.size(): " << pointBuf.size() << endl;
    cout << "objectPoints.size(): " << objectPoints.size() << endl;
    if(pointBuf.size() > 0 && pointBuf.size() == objectPoints.size())
    {
    	cout << "try homography" << endl;
        vector<Point2f> cpointBuf(pointBuf);
        undistortPoints(pointBuf, cpointBuf, intrinsic_matrix, distortion_coeffs);//TODO spr
        homography = cv::findHomography(cpointBuf, objectPoints, homography);
        cout << "homography success" << endl;
    }
    return homography;
}

void AlvarObjectTracker::calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point2f>& corners)
{
    corners.clear();
        for( int i = 0; i < boardSize.height; ++i )
            for( int j = 0; j < boardSize.width; ++j )
                corners.push_back(Point2f(float( j*squareSize ), float( i*squareSize )));
}

vector<Point2f> AlvarObjectTracker::trackInPicturePixels(Mat picture){
	vector<Point2f> res;
	cout << "track0" << endl;
		IplImage image = picture;
		IplImage *image_tmp = &image;
		String result;
		cout << "track2" << endl;
		Mat imageUndistortedMat;
		vector<Point2f> pointBuf;
		undistort(picture, imageUndistortedMat, cameraMatrix, distCoeffs);
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
			vector<Point2f> udorgPoint(orgPoint.size());
			cout << "try undistort" << endl;
			undistortPoints(orgPoint, udorgPoint, cameraMatrix, distCoeffs);
			res.push_back(Point2f(udorgPoint[0].x,udorgPoint[0].y));
			res.push_back(Point2f(udorgPoint[1].x,udorgPoint[1].y));
			res.push_back(Point2f(udorgPoint[2].x,udorgPoint[2].y));
			/*res.push_back(Point2f(orgPoint[0].x,orgPoint[0].y));
			res.push_back(Point2f(orgPoint[1].x,orgPoint[1].y));
			res.push_back(Point2f(orgPoint[2].x,orgPoint[2].y));*/
		}
	return res;
}
