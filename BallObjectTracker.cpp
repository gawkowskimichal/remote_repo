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
		result += boost::lexical_cast<std::string>(i) + " " +
					"x: " + boost::lexical_cast<std::string>(rorgPoint[0].x) + " " +
					"y: " + boost::lexical_cast<std::string>(rorgPoint[0].y) + " " +
					"radius: " + boost::lexical_cast<std::string>(circles[i][2]) + " " +
					time + " "+
					"\n";
	  }
    result += "\n";
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
