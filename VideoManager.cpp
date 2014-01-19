/*
 * VideoManager.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "VideoManager.h"

VideoManager::VideoManager() {
	connector1 = new VideoConnector();
	connector2 = new VideoConnector();

}

VideoManager::~VideoManager() {
	// TODO Auto-generated destructor stub
}

void VideoManager::CaptureFrom( int i ){
	int result = 0;
	result = connector1->captureVideo(i);
}

void VideoManager::CaptureFromBoth(){
	thread task_for_cam1(&VideoManager::CaptureFrom, this, 0);
	thread task_for_cam2(&VideoManager::CaptureFrom, this, 1);
	task_for_cam1.join();
	task_for_cam2.join();
};

void VideoManager::CaptureMultiple(){
	int result = 0;
	result = connector1->captureMultipleVideo();

}
