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
	ReaderCounter = 0;
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
		calib_files.push_back(conf.getValueByKey("calib_file_"+boost::lexical_cast<std::string>(i)));
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
	return this->connector->readImageFromFile(path);
}

void VideoManager::TrackFromFiles(Configuration conf){
	cout << "tracking1" << endl;
	bool file_ends = false;
	tracker = new AlvarObjectTracker(conf,conf.getValueByKey("calib_file_0"));
	vector<std::string> real_positions;
	vector<string> tokens;
	//initTrackers(conf);
	cout << "tracking2" << endl;
	string working_dir = conf.getValueByKey("pathToWorkDir");
	string info_file = conf.getValueByKey("pathToTimestampFile");
	ifstream info_file_str;
	info_file_str.open(info_file.c_str());
	if (info_file_str.is_open()){
		cout << "File not open!" << endl;
	}
	string line;
	string pos;
	string time;
	cout << "tracking3" << endl;
	while (!file_ends){
		if(!getline(info_file_str,line)){
			cout << "tracking4" << endl;
			boost::mutex::scoped_lock lock(this->connector->file_mutex);
			this->connector->images_to_write.wait(lock);
			info_file_str.clear();
			info_file_str.open(info_file.c_str());
		}
		if (line == "_END"){
			cout << "tracking END" << endl;
			file_ends = true;
		} else {
			if (line != ""){
				cout << "tracking5" << endl;
				cout << "line: " << line << endl;
				boost::split(tokens,line,boost::is_any_of(" ")); //filename, timestamp, counter
				cout << tokens.size() << endl;
				for (int i = 0; i < 3; i++){
					cout << tokens[i] << endl;
				}
				cout << "tracking6" << endl;
				time = tokens[1];
				pos = tracker->trackInPicture(readImageFromFile(tokens[0]),time);
				cout << "tracking7" << endl;
				real_positions.push_back(pos);
				cout << "tracking8" << endl;
			}
		}
	}
	for (int i = 0; i < real_positions.size(); i++){
		cout << real_positions[i] << endl;
	}
};

void VideoManager::TrackMultipleFromFiles(Configuration conf){
	initTrackers(conf);
	bool file_ends = false;
	int camera_index = 0;
	vector<vector<std::string>> real_positions;
	vector<string> tokens;
	vector<string> small_tokens;
	string working_dir = conf.getValueByKey("pathToWorkDir");
	string info_file = conf.getValueByKey("pathToTimestampFile");
	ifstream info_file_str;
	info_file_str.open(info_file.c_str());
	string line;
		while (!file_ends){
			if(!getline(info_file_str,line)){
				boost::mutex::scoped_lock lock(this->connector->file_mutex);
				this->connector->images_to_write.wait(lock);
			}
			if (line == "_END"){
				file_ends = true;
			} else {
				if (line != ""){
					boost::split(tokens,line,boost::is_any_of(" ")); //filename, timestamp, counter
					boost::split(small_tokens,tokens[2],boost::is_any_of("_"));
					camera_index = atoi(small_tokens[1].c_str());
					Mat picture = readImageFromFile(tokens[0]);
					real_positions[camera_index].push_back(trackers[camera_index]->trackInPicture(picture,tokens[1]));
				}
			}
		}
};

void VideoManager::CaptureAndTrack(Configuration conf){
	boost::thread_group group;
	cout << "Start capture!" << endl;
	VideoManager::TrackFromFiles(conf);
	capturing_thread = boost::thread(&VideoManager::CaptureToFiles, this, conf);
	cout << "Start tracking!" << endl;
	tracking_thread = boost::thread(&VideoManager::TrackFromFiles, this, conf);
	group.add_thread(&capturing_thread);
	group.add_thread(&tracking_thread);
	group.join_all();
	cout << "End all!" << endl;
};

void VideoManager::CaptureAndTrackMultiple(Configuration conf){
	boost::thread_group group;
	cout << "Start capture!" << endl;
	capturing_thread = boost::thread(&VideoManager::CaptureMultipleToFiles, this, conf);
	cout << "Start tracking!" << endl;
	tracking_thread = boost::thread(&VideoManager::TrackMultipleFromFiles, this, conf);
	group.add_thread(&capturing_thread);
	group.add_thread(&tracking_thread);
	group.join_all();
	cout << "End all!" << endl;
};
