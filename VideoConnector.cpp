/*
 * VideoConnector.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "VideoConnector.h"
using namespace std;
using namespace cv;
using namespace TracingFramework;

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

vector<std::pair<Mat,String>> VideoConnector::captureVideoToVector(){
	Mat frame, frameCopy;
	vector<std::pair<Mat,String>> result;
	for(;;){
			frame = cvQueryFrame( cameras.at(0)->cam );
			frameCopy = frame.clone();
			if (!frameCopy.empty()){
				cv::imshow("Video", frameCopy );
			}
			result.push_back(std::make_pair(frameCopy,getTime()));
			char c = (char)waitKey(33);
			if( c == 27 ){
				destroyWindow("Video");
				break;
			}
	}
	return result;
};

vector<vector<std::pair<Mat,String>>> VideoConnector::captureMultipleVideoToVectors(){
	Mat frame, frameCopy;
	vector<Mat> frames;
	vector<std::pair<Mat,String>> framesCopies;
	vector<vector<std::pair<Mat,String>>> result;

	for(;;){
		frame = cvQueryFrame( cameras.at(0)->cam );
		frameCopy = frame.clone();
		if (!frameCopy.empty()){
			cv::imshow("captureMultipleVideoToVectors", frameCopy );
			for(int j = 0; j < cameras.size(); j++){
					frames.push_back(cvQueryFrame(cameras[j]->cam));
					framesCopies.push_back(std::make_pair(frames[j].clone(),getTime()));
			}
			result.push_back(framesCopies);
			frames.clear();
			framesCopies.clear();
		}
		char c = (char)waitKey(33);
		if( c == 27 ){
			destroyWindow("captureMultipleVideoToVectors");
			break;
		}
	}
	return result;
};

int VideoConnector::captureVideoToFilesWithInfo(Configuration conf){
	int res = 0;
	Mat frame, frameCopy;
	int counter = 0;
	string path_to_dir = conf.getValueByKey("pathToWorkDir");
	string path_to_info_file = conf.getValueByKey("pathToTimestampFile");
	string time_s;
	string fileName;
	for(;;){
			frame = cvQueryFrame( cameras.at(0)->cam );
			frameCopy = frame.clone();
			if (!frameCopy.empty()){
				cv::imshow("Video", frameCopy );
			}
			time_s = getTime();
			fileName = path_to_dir+"_img_"+boost::lexical_cast<std::string>(counter)+".jpg";
			saveImageToFile(frameCopy,fileName);
			saveInfoToFile(path_to_info_file, fileName + " " + time_s+" "+boost::lexical_cast<std::string>(counter));
			char c = (char)waitKey(33);
			if( c == 27 ){
				destroyWindow("Video");
				break;
			}
			counter++;
	}
	saveInfoToFile(path_to_info_file, "_END");
	return res;
};

int VideoConnector::captureMultipleVideoToFilesWithInfo(Configuration conf){
	int res = 0;
	Mat frame, frameCopy;
	vector<Mat> frames;
	vector<Mat> framesCopies;
	string path_to_dir = conf.getValueByKey("pathToWorkDir");
	string path_to_info_file = conf.getValueByKey("pathToTimestampFile");
	int counter=0;
	string time_s;
	string fileName;
	for(;;){
		frame = cvQueryFrame( cameras.at(0)->cam );
		frameCopy = frame.clone();
		if (!frameCopy.empty()){
			cv::imshow("captureMultipleVideoToVectors", frameCopy );
			for(int j = 0; j < cameras.size(); j++){
					frames.push_back(cvQueryFrame(cameras[j]->cam));
					framesCopies.push_back(frames[j].clone());
					time_s = getTime();
					for(std::vector<Mat>::iterator it = framesCopies.begin(); it != framesCopies.end(); ++it){
						fileName = path_to_dir+"_img_"+boost::lexical_cast<std::string>(counter)+"_"+boost::lexical_cast<std::string>(j)+".jpg";
						saveImageToFile((*it),fileName);
						saveInfoToFile(path_to_info_file,fileName + " " + time_s+" "+boost::lexical_cast<std::string>(counter)+"_"+boost::lexical_cast<std::string>(j));
					}
			}
			frames.clear();
			framesCopies.clear();
		}
		char c = (char)waitKey(33);
		if( c == 27 ){
			destroyWindow("captureMultipleVideoToVectors");
			break;
		}
		counter++;
	}
	saveInfoToFile(path_to_info_file, "_END");
	return res;
};

void VideoConnector::captureVideoToFile(int i, Configuration conf){
		CvCapture *cap;
		Mat frame, frameCopy;
		cap = cameras[i]->cam;
		if(!cap) cout << "No camera detected" << endl;
		if( cap ){
			String output_file = "out" + boost::lexical_cast<std::string>(i) + ".avi";
			double width = atof(conf.getValueByKey("resolution_y").c_str());
			double height = atof(conf.getValueByKey("resolution_x").c_str());
			VideoWriter video(output_file,CV_FOURCC('M','J','P','G'),1, Size(width,height),true);
			for(;;){
				frame = cvQueryFrame( cap );
				frameCopy = frame.clone();
				if (!frameCopy.empty()){
					cv::imshow("VideoCapture", frameCopy );
					video.write(frameCopy);
				}
				char c = (char)waitKey(30);
				if( c == 27 ){
					destroyWindow("VideoCapture");
					break;
				}
			}
			video.release();
		}
			    return;
}

void VideoConnector::captureMultipleVideoToFiles(Configuration conf){
		Mat frame, frameCopy;
		vector<Mat> frames;
		vector<Mat> framesCopies;
		vector<VideoWriter*> writers;
		VideoWriter *temp;
		int i = 0;
		double width = atof(conf.getValueByKey("resolution_y").c_str());
		double height = atof(conf.getValueByKey("resolution_x").c_str());
		for(std::vector<Camera*>::iterator it = cameras.begin(); it != cameras.end(); ++it){
			String fileName = "out_multiple_"+boost::lexical_cast<std::string>(i)+".avi";
			temp = new VideoWriter(fileName,CV_FOURCC('M','J','P','G'),5, Size(width,height),true);
			writers.push_back(temp);
			i++;
		}
		for(;;){
			frame = cvQueryFrame( cameras.at(0)->cam );
			frameCopy = frame.clone();
			if (!frameCopy.empty()){
				cv::imshow("VideoMultipleCapture", frameCopy );
				for(int j = 0; j < cameras.size(); j++){
						frames.push_back(cvQueryFrame(cameras[j]->cam));
						framesCopies.push_back(frames[j].clone());
						writers[j]->write(framesCopies[j]);
						frames.clear();
						framesCopies.clear();
				}
			}
			char c = (char)waitKey(33);
			if( c == 27 ){
				destroyWindow("VideoMultipleCapture");
				break;
			}
		}
		return;
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

void VideoConnector::saveInfoToFile(String info, String path){
	try{
		std::ofstream outfile;
        outfile.open(path.c_str(), std::ios_base::app);
		outfile << info << endl;
	} catch(string obj) {
		cout << "Error: unable to save info to file:" << path << endl;
		cout << obj << endl;
	}
};

void VideoConnector::saveImageToFile(Mat m, String path){
	try{
		imwrite(path,m);
	} catch(string obj) {
		cout << "Error: unable to save Mat to file:" << path << endl;
		cout << obj << endl;
	}
};

Mat VideoConnector::readImageFromFile(String path){
	Mat res;
	try{
		res = imread(path,CV_LOAD_IMAGE_COLOR);
	} catch(string obj){
		cout << "Error: unable to load Mat from file:" << path << endl;
		cout << obj << endl;
	}
}

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

string VideoConnector::getTime(){
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime (&rawtime);

	const int size = 16;
	char buffer [size];

	strftime (buffer,size,"%Y%b%d%H%M%S",timeinfo);
	std::string time(buffer, size-1);

	return time;
}
