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

vector<vector<Point3f>> VideoManager::TrackInMultipleTriangulate(Configuration conf, vector<vector<std::pair<Mat,string>>> pictures){
	initTrackers(conf);
	vector<vector<vector<Point2f>>> pixelPositions;
	vector<vector<Point3f>> realPositions;
	vector<string> tokens;
	vector<string> small_tokens;
	int numberOfFrames = pictures.at(0).size();
	vector<Mat> frameFromCameras;
	Mat P1 = readMat(conf,"P1");
	Mat P2 = readMat(conf,"P2");
	for (int c = 0; c < this->connector->CameraCounter; c++){
		vector<vector<Point2f>> cameraVector;
		pixelPositions.push_back(cameraVector);
	}
	for (int i = 0; i < numberOfFrames; i++){
		Mat realPos;
		for (int j = 0; j < pictures.size(); j++){
			frameFromCameras.push_back(pictures.at(j).at(i).first);
			pixelPositions.at(j).push_back(this->trackers.at(j)->trackInPicturePixels(pictures.at(j).at(i).first));
		}
		frameFromCameras.clear();
	}
	for (int j = 0; j < numberOfFrames; j++){
				vector<Point3f> realPoints;
				triangulatePoints(P1,P2,pixelPositions.at(0),pixelPositions.at(1),realPoints);
				realPositions.push_back(realPoints);
				realPoints.clear();
	}
	return realPositions;
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
	bool file_ends = false;
	int last_postion = 0;
	tracker = new AlvarObjectTracker(conf,conf.getValueByKey("calib_file_0"));
	vector<std::string> real_positions;
	vector<string> tokens;
	boost::mutex::scoped_lock lock(this->connector->file_mutex);
	//initTrackers(conf);
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
	while (!file_ends){
		if(!getline(info_file_str,line)){
			this->connector->images_to_write.wait(lock);
			info_file_str.close();
			info_file_str.clear();
			info_file_str.open(info_file.c_str());
			info_file_str.seekg(last_postion) ;
		}
		if (line == "_END"){
			file_ends = true;
		} else {
			if (line != ""){
				boost::split(tokens,line,boost::is_any_of(" ")); //filename, timestamp, counter
				cout << tokens.size() << endl;
				time = tokens[1];
				pos = tracker->trackInPicture(readImageFromFile(tokens[0]),time);
				real_positions.push_back(pos);
			}
		}
		last_postion = info_file_str.tellg();
	}
	for (int i = 0; i < real_positions.size(); i++){
		cout << real_positions[i] << endl;
	}
};

void VideoManager::TrackMultipleFromFiles(Configuration conf){
	initTrackers(conf);
	bool file_ends = false;
	int camera_index = 0;
	int last_postion = 0;
	boost::mutex::scoped_lock lock(this->connector->file_mutex);
	vector<vector<std::string>> real_positions;
	vector<string> tokens;
	vector<string> small_tokens;
	string working_dir = conf.getValueByKey("pathToWorkDir");
	string info_file = conf.getValueByKey("pathToTimestampFile");
	ifstream info_file_str;
	info_file_str.open(info_file.c_str());
	string line;
	for (int i = 0; i < this->connector->cameras.size(); i++){
		vector<string> camera_result;
		real_positions.push_back(camera_result);
	}
	while (!file_ends){
		if(!getline(info_file_str,line)){
			this->connector->images_to_write.wait(lock);
			info_file_str.close();
			info_file_str.clear();
			info_file_str.open(info_file.c_str());
			info_file_str.seekg(last_postion) ;
			cout << "Wall hit" << endl;
		}
		if (line == "_END"){
			file_ends = true;
			cout << "End of file found" << endl;
		} else {
			if (line != ""){
				boost::split(tokens,line,boost::is_any_of(" ")); //filename, timestamp, counter
				boost::split(small_tokens,tokens[2],boost::is_any_of("_"));
				camera_index = atoi(small_tokens[1].c_str());
				Mat picture = readImageFromFile(tokens[0]);
				real_positions[camera_index].push_back(trackers[camera_index]->trackInPicture(picture,tokens[1]));
				cout << "Tracked sth" << endl;
			}
		}
		last_postion = info_file_str.tellg();
	}
	for (int i = 0; i < real_positions.size(); i++){
		for (int j = 0; j<real_positions[i].size(); j++){
			cout << i << " " << j << endl;
			cout << real_positions[i][j] << endl;
		}
	}
};

void VideoManager::TrackMultipleFromFilesTriangulate(Configuration conf){
	initTrackers(conf);
	bool file_ends = false;
	int camera_index = 0;
	int last_postion = 0;
	int camera_count = this->connector->CameraCounter;
	int numberOfFrames = 0;
	Mat P1 = readMat(conf,"P1");
	Mat P2 = readMat(conf,"P2");
	boost::mutex::scoped_lock lock(this->connector->file_mutex);
	vector<vector<vector<Point2f>>> pixel_positions;
	vector<vector<Point3f>> real_positions;
	vector<string> tokens;
	vector<string> small_tokens;
	vector<vector<Mat>> framesFromCameras;
	for (int i = 0; i < camera_count; i++){
		vector<Mat> cameraVector;
		framesFromCameras.push_back(cameraVector);
	}
	string working_dir = conf.getValueByKey("pathToWorkDir");
	string info_file = conf.getValueByKey("pathToTimestampFile");
	ifstream info_file_str;
	info_file_str.open(info_file.c_str());
	string line;
	for (int i = 0; i < this->connector->cameras.size(); i++){
		vector<vector<Point2f>> camera_result;
		pixel_positions.push_back(camera_result);
	}
	while (!file_ends){
		if(!getline(info_file_str,line)){
			this->connector->images_to_write.wait(lock);
			info_file_str.close();
			info_file_str.clear();
			info_file_str.open(info_file.c_str());
			info_file_str.seekg(last_postion) ;
			cout << "Wall hit" << endl;
		}
		if (line == "_END"){
			file_ends = true;
			cout << "End of file found" << endl;
		} else {
			if (line != ""){
				boost::split(tokens,line,boost::is_any_of(" ")); //filename, timestamp, counter
				boost::split(small_tokens,tokens[2],boost::is_any_of("_"));
				camera_index = atoi(small_tokens[1].c_str());
				if (numberOfFrames < atoi(small_tokens[0].c_str())){
					numberOfFrames = atoi(small_tokens[0].c_str());
				}
				Mat picture = readImageFromFile(tokens[0]);
				pixel_positions[camera_index].push_back(trackers[camera_index]->trackInPicturePixels(picture));
				cout << "Tracked sth" << endl;
			}
		}
		last_postion = info_file_str.tellg();
	}
	// get real_pos info
	for (int j = 0; j < numberOfFrames; j++){
					vector<Point3f> realPoints;
					triangulatePoints(P1,P2,pixel_positions.at(0),pixel_positions.at(1),realPoints);
					real_positions.push_back(realPoints);
					realPoints.clear();
	}
};


void VideoManager::CaptureAndTrack(Configuration conf){
	boost::thread_group group;
	cout << "Start capture!" << endl;
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
void VideoManager::CaptureAndTriangulate(Configuration conf){
	boost::thread_group group;
	cout << "Start capture" << endl;
	capturing_thread = boost::thread(&VideoManager::CaptureMultipleToFiles, this, conf);
	cout << "Start tracking!" << endl;
	tracking_thread = boost::thread(&VideoManager::TrackMultipleFromFilesTriangulate, this, conf);
	group.add_thread(&capturing_thread);
	group.add_thread(&tracking_thread);
	group.join_all();
	cout << "END all!" << endl;
};

void VideoManager::initQMatrix(Configuration conf, bool reinit){
	if (Q.data == NULL || reinit){
		String pathToQ = conf.getValueByKey("Q_file");
	    cv::FileStorage fs;
	    fs.open( pathToQ, FileStorage::READ );
	    bool fsIsOpened = fs.isOpened();
	    if(fsIsOpened)
	    {
	        fs["Q"] >> Q;
	    }
	}
};


Mat VideoManager::readMat(Configuration conf,string name){
	String pathToName = conf.getValueByKey(name);
	cv::FileStorage fs;
	fs.open(pathToName,FileStorage::READ);
	Mat output;
	bool fsIsOpened = fs.isOpened();
	if(fsIsOpened){
		fs[name] >> output;
	}
	return output;
}

Point3f VideoManager::triangulatePointsFromMultipleCameras(Configuration conf, int cameraOneIndex, int cameraTwoIndex,
		Point2f firstImgPoint, Point2f secondImgPoint){
	initQMatrix(conf,false);
	Point3f res(0,0,0);
	if (this->Q.rows == 4 &&this->Q.cols == 4){
		// w tym momencie firstImgPoint i secondImgPoint to punkty ze wspolrzednymi z obrazkow
		// z usuniętymi dystorsjami
		double d = abs(secondImgPoint.x - firstImgPoint.x);
		double X = firstImgPoint.x * Q.at<float>(0,0)+ Q.at<float>(0, 3);
		double Y = firstImgPoint.y * Q.at<float>(1, 1) + Q.at<float>(1, 3);
		double Z = Q.at<float>(2, 3);
		double W = d * Q.at<float>(3, 2) + Q.at<float>(3, 3);
		res.x = X/W;
		res.y = Y/W;
		res.z = Z/W;
	} else {
		cout << "Macierz reprojekcji ma złe wymiary!" << endl;
	}
	return res;
};

void VideoManager::test_stereo_tracking(Configuration conf){
	initTrackers(conf);
	CaptureAndTriangulate(conf);
};
