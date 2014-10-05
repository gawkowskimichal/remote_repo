/*
 * VideoManager.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "VideoManager.h"

VideoManager::VideoManager(Configuration conf) {
	connector = new VideoConnector();
	connector->initCameras(conf);
}

VideoManager::~VideoManager() {
	// TODO Auto-generated destructor stub
}

void VideoManager::TrackInPicture(Mat *picture){
	tracker->trackInPicture(picture);
};

void VideoManager::TrackInPictures(vector<Mat*> pictures){
	tracker->trackInPictures(pictures);
};

void VideoManager::TrackInVideo(String fileName){
	tracker->trackInVideo(fileName);
};

void VideoManager::CaptureFrom( int i ){
	int result = 0;
	result = connector->captureVideo(i);
}

void VideoManager::getCalibrationMaterial(Configuration conf, int i){
	connector->getCalibrationMaterial(conf,i);
}

void VideoManager::CaptureMultiple(){
	int result = 0;
	result = connector->captureMultipleVideo();
}

void VideoManager::getCalibrationMultipleMaterial(Configuration conf){
	connector->getCalibrationMultipleMaterial(conf);
};

void VideoManager::shutdown(){
	connector->shutdownCameras();
}
