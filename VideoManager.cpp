/*
 * VideoManager.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "VideoManager.h"
using namespace TracingFramework;

VideoManager::VideoManager(Configuration conf) {
	connector = new VideoConnector();
	connector->initCameras(conf);
}

VideoManager::~VideoManager() {
	// TODO Auto-generated destructor stub
}

void VideoManager::TrackInPicture(Mat picture, String time){
	tracker->trackInPicture(picture,time);
};

void VideoManager::TrackInPictures(vector<std::pair<Mat,String>> pictures){
	tracker->trackInPictures(pictures);
};

void VideoManager::TrackInVideo(String fileName){
	tracker->trackInVideo(fileName);
};

void VideoManager::CaptureFrom( int i ){
	connector->captureVideo(i);
}

void VideoManager::getCalibrationMaterial(Configuration conf, int i){
	connector->getCalibrationMaterial(conf,i);
}

void VideoManager::CaptureMultiple(){
	connector->captureMultipleVideo();
}

void VideoManager::getCalibrationMultipleMaterial(Configuration conf){
	connector->getCalibrationMultipleMaterial(conf);
};

void VideoManager::shutdown(){
	connector->shutdownCameras();
}

void VideoManager::captureVideoToFileFromCam(int i, Configuration conf){
	connector->captureVideoToFile(i,conf);
};

void VideoManager::captureVideoToFilesFromCams(Configuration conf){
	connector->captureMultipleVideoToFiles(conf);
};

vector<std::pair<Mat,String>> VideoManager::captureToMemory(int i, Configuration conf){
	return connector->captureVideoToVector();
};
vector<vector<std::pair<Mat,String>>> VideoManager::captureToMemoryMultiple(Configuration conf){
	return connector->captureMultipleVideoToVectors();
};

void VideoManager::CaptureToFiles(Configuration conf){
	connector->captureVideoToFilesWithInfo(conf);
};

void VideoManager::CaptureMultipleToFiles(Configuration conf){
	connector->captureMultipleVideoToFilesWithInfo(conf);
};

void VideoManager::initTrackers(Configuration conf){
	string marker_type = conf.getValueByKey("marker_type");
	vector<string> calib_files;
	for (int i = 0; i < connector->cameras.size(); i++){
		calib_files.push_back(conf.getValueByKey("calib_"+boost::lexical_cast<std::string>(i)));
	}
	if (marker_type == "alvar"){
		for (int i = 0; i < connector->cameras.size(); i++){
			trackers.push_back(new AlvarObjectTracker(conf,calib_files[i]));
		}
	} else if (marker_type == "ball"){
		for (int i = 0; i < connector->cameras.size(); i++){
			trackers.push_back(new BallObjectTracker(conf,calib_files[i]));
		}
	}
};

Mat VideoManager::readImageFromFile(string path){
	Mat res = this->connector->readImageFromFile(path);
	return res;
}

void VideoManager::TrackFromFiles(Configuration conf){
	int last_index = 0;
	bool file_not_empty = false;
	vector<std::string> real_positions;
	vector<string> tokens;
	initTrackers(conf);
	tracker = trackers[0];
	string working_dir = conf.getValueByKey("pathToWorkDir");
	string info_file = conf.getValueByKey("pathToTimestampFile");
	ifstream info_file_str;
	info_file_str.open(info_file.c_str());
	/*
	 * wait till signaled or check semaphore
	 * */
	if (file_not_empty){
		string line;
		while (getline(info_file_str,line)){
			if (line != "_END"){
				boost::split(tokens,line,boost::is_any_of("\t ")); //filename, timestamp, counter
				Mat picture = readImageFromFile(tokens[0]);
				real_positions.push_back(tracker->trackInPicture(picture,tokens[1]));
				last_index = atoi(tokens[2].c_str());
			} else {
				break;
			};
		}
	}
};

void VideoManager::TrackMultipleFromFiles(Configuration conf){
	initTrackers(conf);
};

void VideoManager::CaptureAndTrack(Configuration conf){
	capturing_thread = boost::thread(&VideoManager::CaptureToFiles, this, conf);
	tracking_thread = boost::thread(&VideoManager::TrackFromFiles, this, conf);
};

void VideoManager::CaptureAndTrackMultiple(Configuration conf){
	capturing_thread = boost::thread(&VideoManager::CaptureMultipleToFiles, this, conf);
	tracking_thread = boost::thread(&VideoManager::TrackMultipleFromFiles, this, conf);
};
