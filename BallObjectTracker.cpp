/*
 * BallObjectTracker.cpp
 *
 *  Created on: 12 Oct 2014
 *      Author: mgw
 */

#include "BallObjectTracker.h"

using namespace cv;
using namespace TracingFramework;

BallObjectTracker::BallObjectTracker(Configuration conf, String calibfn) {
	calibrationFile = calibfn;
	config = &conf;
	bool params_read = readCameraParams(calibrationFile, cameraMatrix, distCoeffs);
	if (!params_read){
		cout << "Calib file not read!" << endl;
	}
	maxRadius = atof(config->getValueByKey("ballmaxRadius").c_str());
	minRadius = atof(config->getValueByKey("ballminRadius").c_str());
	CannyParam1 = atof(config->getValueByKey("ballCannyParam1").c_str());
	CannyParam2 = atof(config->getValueByKey("ballCannyParam2").c_str());
	homographyFound = false;
}

BallObjectTracker::~BallObjectTracker() {
	// TODO Auto-generated destructor stub
}

bool BallObjectTracker::readCameraParams( string calib_file, Mat& cameraMatrix, Mat& distCoeffs ) {
    cv::FileStorage fs( calib_file, FileStorage::READ );
    bool fsIsOpened = fs.isOpened();
    if(fsIsOpened)
    {
        fs["Camera_Matrix"] >> cameraMatrix;
        fs["Distortion_Coefficients"] >> distCoeffs;
    }
    return fsIsOpened;
}

String BallObjectTracker::trackInPicture(Mat picture, String time){
	String result;
	Mat pic_gray;
	cvtColor( picture, pic_gray, CV_BGR2GRAY );
	GaussianBlur( pic_gray, pic_gray, Size(9, 9), 2, 2 );
	Mat imageUndistortedMat;
	undistort(picture, imageUndistortedMat, cameraMatrix, distCoeffs);
	vector<Point2f> pointBuf;
    vector<Vec3f> circles;
    HoughCircles( pic_gray, circles, CV_HOUGH_GRADIENT, 1, pic_gray.rows/8, CannyParam1, CannyParam2, minRadius, maxRadius );
    Mat homography;
    	if (!this->homographyFound){
    		homography = findHomography(imageUndistortedMat, Size(9,6), float(2.6), cameraMatrix, distCoeffs, pointBuf);
    		homographyInternal = homography.clone();
    	} else {
    		homography = homographyInternal.clone();
    	}
    cout << "Found circles: " <<  circles.size() << endl;
    if(homography.data != NULL && circles.size() > 0){
		for( size_t i = 0; i < circles.size(); i++ ){
			vector<Point2f> orgPoint;
			orgPoint.push_back(Point2f(circles[i][0], circles[i][1]));
			vector<Point2f> udorgPoint(orgPoint);
			vector<Point2f> rorgPoint(orgPoint);
			undistortPoints(orgPoint, udorgPoint, cameraMatrix, distCoeffs);
			perspectiveTransform( udorgPoint, rorgPoint, homography);
			result += boost::lexical_cast<std::string>(i) + " " +
						"x: " + boost::lexical_cast<std::string>(rorgPoint[0].x) + " " +
						"y: " + boost::lexical_cast<std::string>(rorgPoint[0].y) + " " +
						"radius: " + boost::lexical_cast<std::string>(circles[i][2]) + " " +
						time + " "+
						"\n";
		  }
    }
    result += "\n";
	return result;
};

Mat BallObjectTracker::findHomography(Mat img, Size pattern_size, float squareSize,
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
        homographyFound = true;
        cout << "homography success" << endl;
    }
    return homography;
}

void BallObjectTracker::calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point2f>& corners)
{
    corners.clear();
        for( int i = 0; i < boardSize.height; ++i )
            for( int j = 0; j < boardSize.width; ++j )
                corners.push_back(Point2f(float( j*squareSize ), float( i*squareSize )));
}

vector<std::pair<Point2f,String>> BallObjectTracker::trackInPictureV(Mat picture, String time){
	vector<std::pair<Point2f,String>> result;
	Mat pic_gray;
	cvtColor( picture, pic_gray, CV_BGR2GRAY );
	GaussianBlur( pic_gray, pic_gray, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    HoughCircles( pic_gray, circles, CV_HOUGH_GRADIENT, 1, pic_gray.rows/8, 100, 25, 0, 0 );
    cout << "Found circles: " <<  circles.size() << endl;
    for( size_t i = 0; i < circles.size(); i++ ){
    	vector<Point2f> orgPoint;
		orgPoint.push_back(Point2f(circles[i][0], circles[i][1]));
		vector<Point2f> udorgPoint(orgPoint);
		vector<Point2f> rorgPoint(orgPoint);
		undistortPoints(orgPoint, udorgPoint, cameraMatrix, distCoeffs);
		Mat homography;
		/*homography = findHomography(orgPoint, udorgPoint, homography);
		perspectiveTransform( udorgPoint, rorgPoint, homography);*/
		Point2f partial;
		partial.x = rorgPoint[i].x;
		partial.y = rorgPoint[i].y;
		result.push_back(std::make_pair(partial,time));
	  }
	return result;
};


vector<String> BallObjectTracker::trackInPictures(vector<std::pair<Mat,String>> pictures){
	vector<String> result;
	String partial;
	for (std::vector<std::pair<Mat,String>>::iterator it = pictures.begin(); it != pictures.end(); ++it){
		partial = trackInPicture((*it).first,(*it).second);
		result.push_back(partial);
	}
	return result;
};

vector<vector<std::pair<Point2f,String>>> BallObjectTracker::trackInPicturesV(vector<std::pair<Mat,String>> pictures){
	vector<vector<std::pair<Point2f,String>>> result;
	vector<std::pair<Point2f,String>> partial;
	for (std::vector<std::pair<Mat,String>>::iterator it = pictures.begin(); it != pictures.end(); ++it){
		partial = trackInPictureV((*it).first,(*it).second);
		result.push_back(partial);
	}
	return result;
};

vector<String> BallObjectTracker::trackInVideo(String filename){
	vector<String> result;
	return result;
};

void BallObjectTracker::saveTrackToFile(vector<String> pos, String filename){

};

vector<Point2f> BallObjectTracker::trackInPicturePixels(Mat picture){
	vector<Point2f> res;
	return res;
}

vector<pair<Point2f,String>> BallObjectTracker::trackInPicturePixelsV(Mat picture, String time){
	vector<pair<Point2f,String>> res;
	return res;
}
