/*
 * VideoManager.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "VideoManager.h"

VideoManager::VideoManager() {
	cam1 = new VideoConnector();

}

VideoManager::~VideoManager() {
	// TODO Auto-generated destructor stub
}

void VideoManager::CaptureFrom(int i){
	int result = 0;
	result = cam1->captureVideo();
};
