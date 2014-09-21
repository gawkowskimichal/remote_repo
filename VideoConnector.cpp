/*
 * VideoConnector.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "VideoConnector.h"
using namespace std;
using namespace cv;

VideoConnector::VideoConnector() {
	CameraCounter = 0;
};

VideoConnector::~VideoConnector() {
	// TODO Auto-generated destructor stub
};

int VideoConnector::captureMultipleVideo(){
	   capture1=cvCaptureFromCAM(0);
	   cvSetCaptureProperty( capture1, CV_CAP_PROP_FRAME_WIDTH, 320 );
	   cvSetCaptureProperty( capture1, CV_CAP_PROP_FRAME_HEIGHT, 240 );
	   capture2=cvCaptureFromCAM(1);
	   cvSetCaptureProperty( capture2, CV_CAP_PROP_FRAME_WIDTH, 320 );
	   cvSetCaptureProperty( capture2, CV_CAP_PROP_FRAME_HEIGHT, 240);
	   if(!capture1)
	   {
	      printf("Cannot find device1!\n");
	      exit(0);
	   }
	   if(!capture2)
	   {
	      printf("Cannot find device2!\n");
	      exit(0);
	   }
	   IplImage * IplImage1=0;
	   IplImage * IplImage2=0;
	   cvNamedWindow("CAM1");
	   cvNamedWindow("CAM2");
	   while(1)
	   {
		  IplImage1=cvQueryFrame(capture1);
		  IplImage2=cvQueryFrame(capture2);
	      if(!IplImage1||!IplImage2)
	         break;
	      cvShowImage("CAM1",IplImage1);
	      cvShowImage("CAM2",IplImage2);
	      int key=cvWaitKey(10);
	      if(27==key)
	         break;
	   }
	   cvReleaseCapture(&capture1);
	   cvReleaseCapture(&capture2);
	   cvDestroyWindow("CAM1");
	   cvDestroyWindow("CAM2");

	   return 0;
}

int VideoConnector::captureVideo(int i){
	CvCapture *cap;
	Mat frame, frameCopy;
	cap = cameras[i]->cam;
	if(!cap) cout << "No camera detected" << endl;
	    cvNamedWindow( "result", 1 );
	    if( cap ){
	        cout << "In capture ..." << endl;
	        for(;;){
	            IplImage* iplImg = cvQueryFrame( cap );
	            frame = iplImg;
	            if( frame.empty() ){
	            	 cout << "No image captured" << endl;
	            	 break;
	            }
	            if( iplImg->origin == IPL_ORIGIN_TL )
	            	frame.copyTo( frameCopy );
	            else
	                flip( frame, frameCopy, 0 );
	            cvShowImage( "result", iplImg );
	            if( waitKey( 10 ) >= 0 )
	                cvReleaseCapture( &cap );
	        }
	    }
	    waitKey(0);
	    cvDestroyWindow("result");
	    return 0;
};

Mat VideoConnector::captureSnapshot(int i){
	CvCapture *cap;
	Mat frame, frameCopy;
	cap = cameras[i]->cam;
	if(!cap) cout << "No camera detected" << endl;
	    if( cap ){
	        cout << "In capture ..." << endl;
	        for(;;){
	            frame = cvQueryFrame( cap );
	            frameCopy = frame.clone();
	            if (!frameCopy.empty()){
	            	cv::imshow("Snapshot", frameCopy );
	            }
	            if( waitKey( 10 ) >= 0 ){
	            	destroyWindow("Snapshot");
	            	break;
	            }
	        }
	    }
	    return frameCopy.clone();
}

vector<Mat> VideoConnector::captureMultipleSnapshot(){
	Mat frame, frameCopy;
	vector<Mat> frames;
	vector<Mat> framesCopies;
	vector<Mat> result;
	for(;;){
		frame = cvQueryFrame( cameras.at(0)->cam );
		frameCopy = frame.clone();
		if (!frameCopy.empty()){
			cv::imshow("Snapshot", frameCopy );
		}
		if( waitKey( 10 ) >= 0 ){
			for(std::vector<Camera*>::iterator it = cameras.begin(); it != cameras.end(); ++it){
					frames.push_back(cvQueryFrame((*it)->cam));
			}
			for(std::vector<Mat>::iterator it = frames.begin(); it != frames.end(); ++it){
							framesCopies.push_back((*it).clone());
			}
			destroyWindow("Snapshot");
			break;
		}
	}
	for(std::vector<Mat>::iterator it = framesCopies.begin(); it != framesCopies.end(); ++it){
		result.push_back((*it).clone());
	}
	return result;
};

void VideoConnector::saveImageToFile(Mat m, String path){
	try{
		imwrite(path,m);
	} catch(string obj) {
		cout << "Error: unable to save Mat to file:" << path << endl;
		cout << obj << endl;
	}
};

void VideoConnector::getCalibrationMaterial(Configuration conf, int i){
	String pathToCalibrationDir= conf.getValueByKey("pathToCalibrationDir");
	int numberOfSamples = atoi(conf.getValueByKey("numberOfSamples").c_str());
	String fileName_origin = "cam_" + boost::lexical_cast<std::string>(i) + "_";
	for (int j = 0; j < numberOfSamples; j++){
		Mat frame = captureSnapshot(i);
		String fileName_version = pathToCalibrationDir +"/" +fileName_origin + boost::lexical_cast<std::string>(j) + ".png";
		saveImageToFile(frame,fileName_version);
		cout << "Image saved: " << fileName_version << endl;
	}
}

void VideoConnector::getCalibrationMultipleMaterial(Configuration conf){
	String pathToCalibrationDir= conf.getValueByKey("pathToCalibrationDir");
	int numberOfSamples = atoi(conf.getValueByKey("numberOfSamples").c_str());
	vector<String> files;
	for (int i = 0; i < CameraCounter; i++){
		files.push_back(pathToCalibrationDir +"/"+"cam_"+ boost::lexical_cast<std::string>(i) + "_");
	}
	for (int j = 0; j < numberOfSamples; j++){
		cout << j << endl;
		vector<Mat> snapshots = captureMultipleSnapshot();
		for (int i = 0; i < CameraCounter; i++){
			String fileName = files.at(i) + boost::lexical_cast<std::string>(j) + ".png";
			saveImageToFile(snapshots.at(i),fileName);
			cout << "Image saved: " << fileName << endl;
		}
	}
};

void VideoConnector::initCameras(Configuration conf){
	CameraCounter = getCameraCount(conf);
	for (int i = 0; i < CameraCounter; i++){
		Camera *cam = new Camera(i);
		cameras.push_back(cam);
	}
	setCaptureOptions(conf);
}

void VideoConnector::shutdownCameras(){
	for (int i = 0; i < CameraCounter; i++){
			cvReleaseCapture(&(cameras[i]->cam));
	}
};

void VideoConnector::setCaptureOptions(Configuration conf){
	std::string::size_type sz;
	double width = atof(conf.getValueByKey("resolution_y").c_str());
	double height = atof(conf.getValueByKey("resolution_x").c_str());
	for(std::vector<Camera*>::iterator it = cameras.begin(); it != cameras.end(); ++it){
		(*it)->setCaptureOption(CV_CAP_PROP_FRAME_WIDTH, width);
		(*it)->setCaptureOption(CV_CAP_PROP_FRAME_HEIGHT, height);
	}
}

int VideoConnector::getCameraCount(Configuration conf){
	int result = 0;
	int maxCameras = atoi(conf.getValueByKey("maxCameras").c_str());
	if (maxCameras == 0){
		maxCameras = 2;
	}
	cv::VideoCapture temp;
	for(int i = 0; i< maxCameras; i++){
		temp.open(i);
		if (temp.isOpened()){
			temp.release();
			result++;
		} else {
			return result;
		}
	}
	return result;
}
