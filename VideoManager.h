/*
 * VideoManager.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */
#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_

#include "VideoConnector.h"
#include <thread>

using namespace std;

class VideoManager {
public:
	VideoConnector * connector1;
	VideoConnector * connector2;
	VideoManager();
	virtual ~VideoManager();
	void CaptureFrom( int i );
	void CaptureFromBoth();
	void CaptureMultiple();
};

#endif /* VIDEOMANAGER_H_ */
