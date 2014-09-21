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

void VideoManager::CaptureFrom( int i ){
	int result = 0;
	result = connector->captureVideo(i);
}


void VideoManager::CaptureMultiple(){
	int result = 0;
	result = connector->captureMultipleVideo();
}
